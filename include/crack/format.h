//////////////////////////////////////////////////////////////////////////////
/// \file <crack/format.h>
///
//////////////////////////////////////////////////////////////////////////////

#ifndef CRACK_FORMAT_H

//////////////////////////////////////////////////////////////////////////////
///
/// SECTIONS: Index of this file
///
/// [SECTION] Include Mess
/// [SECTION] Macro Definitions
///
/// Internal Declarations
///
///     [SECTION] Internal : Macros
///     [SECTION] Internal : Forwards
///     [SECTION] Internal : Structures
///     [SECTION] Internal : Functions
///
/// Public Declarations
///
///     [SECTION] Public : Forwards
///     [SECTION] Public : Structures
///     [SECTION] Public : Functions
///
/// [SECTION] Internal Cleanups
///
//////////////////////////////////////////////////////////////////////////////

#define CRACK_FORMAT_H

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Definitions
//-----------------------------------------------------------------------------

#define format(fmt, ...) _FMT_SELECT_HELPER(dummy, ##__VA_ARGS__,                                                   \
                                            _format_has_args, _format_has_args, _format_has_args, _format_has_args, \
                                            _format_has_args, _format_has_args, _format_has_args, _format_has_args, \
                                            _format_has_args, _format_has_args, _format_has_args, _format_has_args, \
                                            _format_has_args, _format_has_args, _format_has_args, _format_no_args)(fmt, ##__VA_ARGS__)

#define format_to(out, size, fmt, ...) _FMT_SELECT_HELPER(dummy, ##__VA_ARGS__,                                                               \
                                                          _format_to_has_args, _format_to_has_args, _format_to_has_args, _format_to_has_args, \
                                                          _format_to_has_args, _format_to_has_args, _format_to_has_args, _format_to_has_args, \
                                                          _format_to_has_args, _format_to_has_args, _format_to_has_args, _format_to_has_args, \
                                                          _format_to_has_args, _format_to_has_args, _format_to_has_args, _format_to_no_args)(out, size, fmt, ##__VA_ARGS__)

#define format_to_n(out, n, fmt, ...)

#define format_size(fmt, ...)

#define fmt_custom(type_name, ptr) _fmt_make_custom(type_name, (const void *)(ptr))

//-----------------------------------------------------------------------------
// [SECTION] Internal Declarations
//-----------------------------------------------------------------------------

#pragma region Private

//-----------------------------------------------------------------------------
// [SECTION] Internal : Macros
//-----------------------------------------------------------------------------

// Generic Selection
#define _MAKE_FORMAT_ARG(x) _Generic((x), \
    format_arg_t: _fmt_identity_arg,      \
    bool: _fmt_make_bool,                 \
    char: _fmt_make_char,                 \
    signed char: _fmt_make_int,           \
    unsigned char: _fmt_make_uint,        \
    short: _fmt_make_int,                 \
    unsigned short: _fmt_make_uint,       \
    int: _fmt_make_int,                   \
    unsigned int: _fmt_make_uint,         \
    long: _fmt_make_int,                  \
    unsigned long: _fmt_make_uint,        \
    long long: _fmt_make_int,             \
    unsigned long long: _fmt_make_uint,   \
    float: _fmt_make_double,              \
    double: _fmt_make_double,             \
    char *: _fmt_make_string,             \
    const char *: _fmt_make_string,       \
    void *: _fmt_make_pointer,            \
    const void *: _fmt_make_pointer,      \
    default: _fmt_make_pointer)(x)

// Variadic Macro Argument Mapping Helpers
#define _FMT_ARG_1(x) _MAKE_FORMAT_ARG(x)
#define _FMT_ARG_2(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_1(__VA_ARGS__)
#define _FMT_ARG_3(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_2(__VA_ARGS__)
#define _FMT_ARG_4(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_3(__VA_ARGS__)
#define _FMT_ARG_5(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_4(__VA_ARGS__)
#define _FMT_ARG_6(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_5(__VA_ARGS__)
#define _FMT_ARG_7(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_6(__VA_ARGS__)
#define _FMT_ARG_8(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_7(__VA_ARGS__)
#define _FMT_ARG_9(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_8(__VA_ARGS__)
#define _FMT_ARG_10(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_9(__VA_ARGS__)
#define _FMT_ARG_11(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_10(__VA_ARGS__)
#define _FMT_ARG_12(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_11(__VA_ARGS__)
#define _FMT_ARG_13(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_12(__VA_ARGS__)
#define _FMT_ARG_14(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_13(__VA_ARGS__)
#define _FMT_ARG_15(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_14(__VA_ARGS__)
#define _FMT_ARG_16(x, ...) _MAKE_FORMAT_ARG(x), _FMT_ARG_15(__VA_ARGS__)

#define _FMT_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define _FMT_COUNT_ARGS(...) _FMT_COUNT_ARGS_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define _FMT_GLUE(x, y) x##y
#define _FMT_CONCAT(x, y) _FMT_GLUE(x, y)

#define _FMT_ARGS_MAP_IMPL(N, ...) _FMT_CONCAT(_FMT_ARG_, N)(__VA_ARGS__)
#define _FMT_ARGS_MAP(...) _FMT_ARGS_MAP_IMPL(_FMT_COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

// Public user-facing macros for formatting
#define _FMT_SELECT_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME

#define _format_no_args(fmt) format_impl(fmt, 0, NULL)
#define _format_has_args(fmt, ...) format_impl(fmt, _FMT_COUNT_ARGS(__VA_ARGS__), (format_arg_t[]){_FMT_ARGS_MAP(__VA_ARGS__)})

#define _format_to_no_args(out, size, fmt) format_to_impl(out, size, fmt, 0, NULL)
#define _format_to_has_args(out, size, fmt, ...) format_to_impl(out, size, fmt, _FMT_COUNT_ARGS(__VA_ARGS__), (format_arg_t[]){_FMT_ARGS_MAP(__VA_ARGS__)})

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Forwards
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Structures
    //-----------------------------------------------------------------------------

    typedef enum format_type_t
    {
        FORMAT_TYPE_BOOL,
        FORMAT_TYPE_CHAR,
        FORMAT_TYPE_INT,
        FORMAT_TYPE_UINT,
        FORMAT_TYPE_DOUBLE,
        FORMAT_TYPE_STRING,
        FORMAT_TYPE_POINTER,
        FORMAT_TYPE_CUSTOM,
    } format_type_t;

    typedef struct format_arg_t
    {
        format_type_t type;
        union
        {
            bool bool_val;
            char char_val;
            long long int_val;
            unsigned long long uint_val;
            double double_val;
            const char *string_val;
            const void *pointer_val;
            struct
            {
                const char *type_name;
                const void *ptr;
            } custom_val;
        } value;
    } format_arg_t;

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Functions
    //-----------------------------------------------------------------------------

    // Core Formatting Engine Functions
    char *format_impl(const char *fmt, size_t argc, const format_arg_t argv[]);
    int format_to_impl(char *out, size_t size, const char *fmt, size_t argc, const format_arg_t argv[]);

    // Helper factory functions for generic conversion
    static inline format_arg_t _fmt_identity_arg(format_arg_t arg) { return arg; }
    static inline format_arg_t _fmt_make_bool(bool val) { return (format_arg_t){.type = FORMAT_TYPE_BOOL, .value.bool_val = val}; }
    static inline format_arg_t _fmt_make_char(char val) { return (format_arg_t){.type = FORMAT_TYPE_CHAR, .value.char_val = val}; }
    static inline format_arg_t _fmt_make_int(long long val) { return (format_arg_t){.type = FORMAT_TYPE_INT, .value.int_val = val}; }
    static inline format_arg_t _fmt_make_uint(unsigned long long val) { return (format_arg_t){.type = FORMAT_TYPE_UINT, .value.uint_val = val}; }
    static inline format_arg_t _fmt_make_double(double val) { return (format_arg_t){.type = FORMAT_TYPE_DOUBLE, .value.double_val = val}; }
    static inline format_arg_t _fmt_make_string(const char *val) { return (format_arg_t){.type = FORMAT_TYPE_STRING, .value.string_val = val}; }
    static inline format_arg_t _fmt_make_pointer(const void *val) { return (format_arg_t){.type = FORMAT_TYPE_POINTER, .value.pointer_val = val}; }
    static inline format_arg_t _fmt_make_custom(const char *type_name, const void *ptr)
    {
        format_arg_t arg;
        arg.type = FORMAT_TYPE_CUSTOM;
        arg.value.custom_val.type_name = type_name;
        arg.value.custom_val.ptr = ptr;
        return arg;
    }

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma endregion // Private

//-----------------------------------------------------------------------------
// [SECTION] Public Declarations
//-----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
/// \defgroup format Formatting
/// \ingroup text
///
/// \{
//////////////////////////////////////////////////////////////////////////////

#pragma region Public

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Forwards
    //-----------------------------------------------------------------------------

    typedef bool (*custom_formatter_t)(const void *value, const char *spec, char *out_out, size_t out_size, size_t *out_written);

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Structures
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Functions
    //-----------------------------------------------------------------------------

    bool format_register_custom(const char *type_name, custom_formatter_t formatter);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma endregion // Public

/// \} group format

//-----------------------------------------------------------------------------
// [SECTION] Internal Cleanups
//-----------------------------------------------------------------------------

#endif // CRACK_FORMAT_H
