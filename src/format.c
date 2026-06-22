#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0600
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(COBRA_USE_PTHREAD) || defined(__unix__) || defined(__APPLE__) || defined(__MINGW32__)
#include <pthread.h>
#define COBRA_HAS_PTHREAD 1
#endif

#include <crack/format.h>

#pragma region Lock Implementation

#define COBRA_MAX_CUSTOM_FORMATTERS 64

typedef struct {
    char type_name[64];
    custom_formatter_t formatter;
} custom_formatter_entry_t;

static custom_formatter_entry_t custom_formatters[COBRA_MAX_CUSTOM_FORMATTERS];
static int custom_formatters_count = 0;

#if defined(_WIN32)
static INIT_ONCE registry_once = INIT_ONCE_STATIC_INIT;
static SRWLOCK registry_lock;
static BOOL CALLBACK init_registry_lock(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context) {
    (void)InitOnce; (void)Parameter; (void)Context;
    InitializeSRWLock(&registry_lock);
    return TRUE;
}
static void lock_registry_shared(void) {
    InitOnceExecuteOnce(&registry_once, init_registry_lock, NULL, NULL);
    AcquireSRWLockShared(&registry_lock);
}
static void unlock_registry_shared(void) {
    ReleaseSRWLockShared(&registry_lock);
}
static void lock_registry_exclusive(void) {
    InitOnceExecuteOnce(&registry_once, init_registry_lock, NULL, NULL);
    AcquireSRWLockExclusive(&registry_lock);
}
static void unlock_registry_exclusive(void) {
    ReleaseSRWLockExclusive(&registry_lock);
}
#elif defined(COBRA_HAS_PTHREAD)
static pthread_mutex_t registry_mutex = PTHREAD_MUTEX_INITIALIZER;
static void lock_registry_shared(void) { pthread_mutex_lock(&registry_mutex); }
static void unlock_registry_shared(void) { pthread_mutex_unlock(&registry_mutex); }
static void lock_registry_exclusive(void) { pthread_mutex_lock(&registry_mutex); }
static void unlock_registry_exclusive(void) { pthread_mutex_unlock(&registry_mutex); }
#else
static void lock_registry_shared(void) {}
static void unlock_registry_shared(void) {}
static void lock_registry_exclusive(void) {}
static void unlock_registry_exclusive(void) {}
#endif

#pragma endregion

#pragma region Helper Functions

typedef struct {
    char fill;
    char align;    // '<', '>', '^', or 0
    char sign;     // '+', '-', ' ', or 0
    bool alt_form; // '#'
    bool zero_pad; // '0'
    int width;     // -1 if not set
    int precision; // -1 if not set
    char type;     // 'd', 'x', 'f', 's', etc. or 0
} format_spec_t;

static bool parse_spec(const char *spec, format_spec_t *out_spec)
{
    out_spec->fill = ' ';
    out_spec->align = 0;
    out_spec->sign = 0;
    out_spec->alt_form = false;
    out_spec->zero_pad = false;
    out_spec->width = -1;
    out_spec->precision = -1;
    out_spec->type = 0;

    if (!spec || *spec == '\0') {
        return true;
    }

    const char *p = spec;

    // Check fill and align
    if (*p != '\0' && (*p != '{' && *p != '}')) {
        if (*(p + 1) == '<' || *(p + 1) == '>' || *(p + 1) == '^') {
            out_spec->fill = *p;
            out_spec->align = *(p + 1);
            p += 2;
        } else if (*p == '<' || *p == '>' || *p == '^') {
            out_spec->align = *p;
            p++;
        }
    }

    // Sign
    if (*p == '+' || *p == '-' || *p == ' ') {
        out_spec->sign = *p;
        p++;
    }

    // Alt form
    if (*p == '#') {
        out_spec->alt_form = true;
        p++;
    }

    // Zero pad
    if (*p == '0') {
        out_spec->zero_pad = true;
        p++;
    }

    // Width
    if (*p >= '0' && *p <= '9') {
        int w = 0;
        while (*p >= '0' && *p <= '9') {
            w = w * 10 + (*p - '0');
            p++;
        }
        out_spec->width = w;
    }

    // Precision
    if (*p == '.') {
        p++;
        int prec = 0;
        while (*p >= '0' && *p <= '9') {
            prec = prec * 10 + (*p - '0');
            p++;
        }
        out_spec->precision = prec;
    }

    // Type
    if (*p != '\0') {
        out_spec->type = *p;
        p++;
    }

    return (*p == '\0');
}

static void format_binary(unsigned long long val, bool alt, char *buf, size_t size) {
    char tmp[128];
    int i = 0;
    if (val == 0) {
        tmp[i++] = '0';
    } else {
        while (val > 0) {
            tmp[i++] = (val & 1) ? '1' : '0';
            val >>= 1;
        }
    }
    size_t len = 0;
    if (alt) {
        if (len < size - 1) buf[len++] = '0';
        if (len < size - 1) buf[len++] = 'b';
    }
    for (int j = i - 1; j >= 0 && len < size - 1; j--) {
        buf[len++] = tmp[j];
    }
    buf[len] = '\0';
}

static custom_formatter_t find_custom_formatter(const char *type_name)
{
    custom_formatter_t formatter = NULL;
    lock_registry_shared();
    for (int i = 0; i < custom_formatters_count; i++) {
        if (strcmp(custom_formatters[i].type_name, type_name) == 0) {
            formatter = custom_formatters[i].formatter;
            break;
        }
    }
    unlock_registry_shared();
    return formatter;
}

#pragma endregion

#pragma region Engine and Destination

typedef struct {
    void (*write)(void *ctx, const char *str, size_t len);
    void *ctx;
    bool error;
} fmt_dest_t;

typedef struct {
    char *data;
    size_t capacity;
    size_t length;
    bool oom;
} dyn_buf_t;

static void dyn_buf_init(dyn_buf_t *buf, size_t initial_cap) {
    buf->capacity = initial_cap;
    buf->length = 0;
    buf->oom = false;
    buf->data = (char *)malloc(initial_cap);
    if (buf->data) {
        buf->data[0] = '\0';
    } else {
        buf->oom = true;
    }
}

static void dyn_buf_append_str(dyn_buf_t *buf, const char *str, size_t len) {
    if (buf->oom) return;
    if (buf->length + len >= buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        while (buf->length + len >= new_cap) {
            new_cap *= 2;
        }
        char *new_data = (char *)realloc(buf->data, new_cap);
        if (!new_data) {
            buf->oom = true;
            return;
        }
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    memcpy(buf->data + buf->length, str, len);
    buf->length += len;
    buf->data[buf->length] = '\0';
}

static void dyn_buf_free(dyn_buf_t *buf) {
    if (buf->data) {
        free(buf->data);
    }
}

static void dest_write_dyn(void *ctx, const char *str, size_t len) {
    dyn_buf_append_str((dyn_buf_t *)ctx, str, len);
}

typedef struct {
    char *buf;
    size_t capacity;
    size_t length;
} fixed_buf_ctx_t;

static void dest_write_fixed(void *ctx, const char *str, size_t len) {
    fixed_buf_ctx_t *f = (fixed_buf_ctx_t *)ctx;
    if (f->length >= f->capacity) return;
    size_t to_write = len;
    if (f->length + to_write >= f->capacity) {
        to_write = f->capacity - 1 - f->length;
    }
    memcpy(f->buf + f->length, str, to_write);
    f->length += to_write;
    f->buf[f->length] = '\0';
}

static bool format_arg(fmt_dest_t *dest, const format_arg_t *arg, const char *spec_str)
{
    format_spec_t spec;
    if (!parse_spec(spec_str, &spec)) {
        return false;
    }

    char val_buf[512];
    val_buf[0] = '\0';
    size_t val_len = 0;

    char prefix_buf[16] = {0};
    size_t prefix_len = 0;

    switch (arg->type) {
        case FORMAT_TYPE_BOOL: {
            bool val = arg->value.bool_val;
            if (spec.type == 'd') {
                val_len = snprintf(val_buf, sizeof(val_buf), "%d", val ? 1 : 0);
            } else {
                val_len = snprintf(val_buf, sizeof(val_buf), "%s", val ? "true" : "false");
            }
            break;
        }
        case FORMAT_TYPE_CHAR: {
            char val = arg->value.char_val;
            if (spec.type == 'd') {
                val_len = snprintf(val_buf, sizeof(val_buf), "%d", val);
            } else {
                val_buf[0] = val;
                val_buf[1] = '\0';
                val_len = 1;
            }
            break;
        }
        case FORMAT_TYPE_INT:
        case FORMAT_TYPE_UINT: {
            unsigned long long val;
            bool is_signed = (arg->type == FORMAT_TYPE_INT);
            long long signed_val = is_signed ? arg->value.int_val : 0;
            val = is_signed ? (unsigned long long)(signed_val < 0 ? -signed_val : signed_val) : arg->value.uint_val;

            if (is_signed && signed_val < 0) {
                prefix_buf[prefix_len++] = '-';
            } else if (spec.sign == '+') {
                prefix_buf[prefix_len++] = '+';
            } else if (spec.sign == ' ') {
                prefix_buf[prefix_len++] = ' ';
            }

            char representation = spec.type ? spec.type : 'd';
            if (representation == 'x' || representation == 'X') {
                if (spec.alt_form) {
                    prefix_buf[prefix_len++] = '0';
                    prefix_buf[prefix_len++] = representation;
                }
                val_len = snprintf(val_buf, sizeof(val_buf), representation == 'X' ? "%llX" : "%llx", val);
            } else if (representation == 'o') {
                if (spec.alt_form && val != 0) {
                    prefix_buf[prefix_len++] = '0';
                }
                val_len = snprintf(val_buf, sizeof(val_buf), "%llo", val);
            } else if (representation == 'b' || representation == 'B') {
                if (spec.alt_form) {
                    format_binary(val, false, val_buf, sizeof(val_buf));
                    val_len = strlen(val_buf);
                    prefix_buf[prefix_len++] = '0';
                    prefix_buf[prefix_len++] = representation;
                } else {
                    format_binary(val, false, val_buf, sizeof(val_buf));
                    val_len = strlen(val_buf);
                }
            } else {
                val_len = snprintf(val_buf, sizeof(val_buf), "%llu", val);
            }
            break;
        }
        case FORMAT_TYPE_DOUBLE: {
            double val = arg->value.double_val;
            
            if (val < 0) {
                prefix_buf[prefix_len++] = '-';
                val = -val;
            } else if (spec.sign == '+') {
                prefix_buf[prefix_len++] = '+';
            } else if (spec.sign == ' ') {
                prefix_buf[prefix_len++] = ' ';
            }

            char fmt_str[32];
            int prec = spec.precision >= 0 ? spec.precision : 6;
            char representation = spec.type ? spec.type : 'f';
            snprintf(fmt_str, sizeof(fmt_str), "%%.%d%c", prec, representation);
            val_len = snprintf(val_buf, sizeof(val_buf), fmt_str, val);
            break;
        }
        case FORMAT_TYPE_STRING: {
            const char *val = arg->value.string_val ? arg->value.string_val : "nil";
            size_t len = strlen(val);
            if (spec.precision >= 0 && (size_t)spec.precision < len) {
                len = spec.precision;
            }
            if (len >= sizeof(val_buf)) {
                len = sizeof(val_buf) - 1;
            }
            memcpy(val_buf, val, len);
            val_buf[len] = '\0';
            val_len = len;
            break;
        }
        case FORMAT_TYPE_POINTER: {
            const void *val = arg->value.pointer_val;
            val_len = snprintf(val_buf, sizeof(val_buf), "%p", val);
            break;
        }
        case FORMAT_TYPE_CUSTOM: {
            custom_formatter_t formatter = find_custom_formatter(arg->value.custom_val.type_name);
            if (formatter) {
                size_t written = 0;
                if (!formatter(arg->value.custom_val.ptr, spec_str, val_buf, sizeof(val_buf), &written)) {
                    return false;
                }
                val_len = written;
            } else {
                val_len = snprintf(val_buf, sizeof(val_buf), "[custom %s at %p]", arg->value.custom_val.type_name, arg->value.custom_val.ptr);
            }
            break;
        }
        default:
            return false;
    }

    size_t total_len = prefix_len + val_len;
    if (spec.width >= 0 && (size_t)spec.width > total_len) {
        size_t pad_len = (size_t)spec.width - total_len;
        
        if (spec.zero_pad && !spec.align) {
            dest->write(dest->ctx, prefix_buf, prefix_len);
            for (size_t i = 0; i < pad_len; i++) {
                dest->write(dest->ctx, "0", 1);
            }
            dest->write(dest->ctx, val_buf, val_len);
        } else {
            char align = spec.align ? spec.align : ((arg->type == FORMAT_TYPE_INT || arg->type == FORMAT_TYPE_UINT || arg->type == FORMAT_TYPE_DOUBLE) ? '>' : '<');
            
            char pad_char_str[2] = { spec.fill, '\0' };

            if (align == '<') {
                dest->write(dest->ctx, prefix_buf, prefix_len);
                dest->write(dest->ctx, val_buf, val_len);
                for (size_t i = 0; i < pad_len; i++) {
                    dest->write(dest->ctx, pad_char_str, 1);
                }
            } else if (align == '>') {
                for (size_t i = 0; i < pad_len; i++) {
                    dest->write(dest->ctx, pad_char_str, 1);
                }
                dest->write(dest->ctx, prefix_buf, prefix_len);
                dest->write(dest->ctx, val_buf, val_len);
            } else if (align == '^') {
                size_t left_pad = pad_len / 2;
                size_t right_pad = pad_len - left_pad;
                for (size_t i = 0; i < left_pad; i++) {
                    dest->write(dest->ctx, pad_char_str, 1);
                }
                dest->write(dest->ctx, prefix_buf, prefix_len);
                dest->write(dest->ctx, val_buf, val_len);
                for (size_t i = 0; i < right_pad; i++) {
                    dest->write(dest->ctx, pad_char_str, 1);
                }
            }
        }
    } else {
        dest->write(dest->ctx, prefix_buf, prefix_len);
        dest->write(dest->ctx, val_buf, val_len);
    }

    return true;
}

static bool format_engine(fmt_dest_t *dest, const char *fmt, size_t argc, const format_arg_t argv[])
{
    const char *p = fmt;
    size_t next_arg_idx = 0;
    bool has_positional = false;
    bool has_sequential = false;

    while (*p != '\0') {
        if (*p == '{') {
            if (*(p + 1) == '{') {
                dest->write(dest->ctx, "{", 1);
                p += 2;
                continue;
            }

            p++;
            const char *start = p;
            while (*p != '\0' && *p != '}' && *p != ':') {
                p++;
            }

            size_t index = 0;
            bool index_specified = false;

            if (p > start) {
                const char *idx_p = start;
                while (idx_p < p) {
                    if (*idx_p >= '0' && *idx_p <= '9') {
                        index = index * 10 + (*idx_p - '0');
                        index_specified = true;
                    } else {
                        dest->error = true;
                        return false;
                    }
                    idx_p++;
                }
            }

            const char *spec = NULL;
            char spec_buf[64] = {0};
            if (*p == ':') {
                p++;
                const char *spec_start = p;
                while (*p != '\0' && *p != '}') {
                    p++;
                }
                size_t spec_len = p - spec_start;
                if (spec_len < sizeof(spec_buf)) {
                    memcpy(spec_buf, spec_start, spec_len);
                    spec_buf[spec_len] = '\0';
                    spec = spec_buf;
                } else {
                    dest->error = true;
                    return false;
                }
            }

            if (*p == '}') {
                p++;
            } else {
                dest->error = true;
                return false;
            }

            if (index_specified) {
                has_positional = true;
                if (has_sequential) {
                    dest->error = true;
                    return false;
                }
            } else {
                has_sequential = true;
                if (has_positional) {
                    dest->error = true;
                    return false;
                }
                index = next_arg_idx++;
            }

            if (index >= argc) {
                dest->error = true;
                return false;
            }

            if (!format_arg(dest, &argv[index], spec)) {
                dest->error = true;
                return false;
            }
        }
        else if (*p == '}') {
            if (*(p + 1) == '}') {
                dest->write(dest->ctx, "}", 1);
                p += 2;
                continue;
            }
            dest->error = true;
            return false;
        }
        else {
            const char *start = p;
            while (*p != '\0' && *p != '{' && *p != '}') {
                p++;
            }
            dest->write(dest->ctx, start, p - start);
        }
    }

    return true;
}

#pragma endregion

#pragma region Public API

bool format_register_custom(const char *type_name, custom_formatter_t formatter)
{
    if (!type_name || !formatter || strlen(type_name) >= 64) {
        return false;
    }
    lock_registry_exclusive();
    if (custom_formatters_count >= COBRA_MAX_CUSTOM_FORMATTERS) {
        unlock_registry_exclusive();
        return false;
    }
    for (int i = 0; i < custom_formatters_count; i++) {
        if (strcmp(custom_formatters[i].type_name, type_name) == 0) {
            custom_formatters[i].formatter = formatter;
            unlock_registry_exclusive();
            return true;
        }
    }
    strcpy(custom_formatters[custom_formatters_count].type_name, type_name);
    custom_formatters[custom_formatters_count].formatter = formatter;
    custom_formatters_count++;
    unlock_registry_exclusive();
    return true;
}

char *format_impl(const char *fmt, size_t argc, const format_arg_t argv[])
{
    dyn_buf_t db;
    dyn_buf_init(&db, 128);
    if (db.oom) return NULL;
    fmt_dest_t dest = { .write = dest_write_dyn, .ctx = &db, .error = false };
    if (!format_engine(&dest, fmt, argc, argv) || db.oom || dest.error) {
        dyn_buf_free(&db);
        return NULL;
    }
    return db.data;
}

int format_to_impl(char *buf, size_t size, const char *fmt, size_t argc, const format_arg_t argv[])
{
    if (!buf || size == 0) return 0;
    fixed_buf_ctx_t fb = { .buf = buf, .capacity = size, .length = 0 };
    buf[0] = '\0';
    fmt_dest_t dest = { .write = dest_write_fixed, .ctx = &fb, .error = false };
    if (!format_engine(&dest, fmt, argc, argv) || dest.error) {
        return -1;
    }
    return (int)fb.length;
}

#pragma endregion
