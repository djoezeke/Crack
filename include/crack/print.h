#ifndef CRACK_PRINT_H
#define CRACK_PRINT_H

#include <crack/format.h>
#include <errno.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void print_impl(FILE *stream, const char *fmt, size_t argc, const format_arg_t argv[]);
    void println_impl(FILE *stream, const char *fmt, size_t argc, const format_arg_t argv[]);

#define _print_no_args(fmt) print_impl(stdout, fmt, 0, NULL)
#define _print_has_args(fmt, ...) print_impl(stdout, fmt, _FMT_COUNT_ARGS(__VA_ARGS__), (format_arg_t[]){_FMT_ARGS_MAP(__VA_ARGS__)})

#define print(fmt, ...) _FMT_SELECT_HELPER(dummy, ##__VA_ARGS__,                                               \
                                           _print_has_args, _print_has_args, _print_has_args, _print_has_args, \
                                           _print_has_args, _print_has_args, _print_has_args, _print_has_args, \
                                           _print_has_args, _print_has_args, _print_has_args, _print_has_args, \
                                           _print_has_args, _print_has_args, _print_has_args, _print_no_args)(fmt, ##__VA_ARGS__)

#define _println_no_args(fmt) println_impl(stdout, fmt, 0, NULL)
#define _println_has_args(fmt, ...) println_impl(stdout, fmt, _FMT_COUNT_ARGS(__VA_ARGS__), (format_arg_t[]){_FMT_ARGS_MAP(__VA_ARGS__)})

#define println(fmt, ...) _FMT_SELECT_HELPER(dummy, ##__VA_ARGS__,                                                       \
                                             _println_has_args, _println_has_args, _println_has_args, _println_has_args, \
                                             _println_has_args, _println_has_args, _println_has_args, _println_has_args, \
                                             _println_has_args, _println_has_args, _println_has_args, _println_has_args, \
                                             _println_has_args, _println_has_args, _println_has_args, _println_no_args)(fmt, ##__VA_ARGS__)

#define _print_to_no_args(stream, fmt) print_impl(stream, fmt, 0, NULL)
#define _print_to_has_args(stream, fmt, ...) print_impl(stream, fmt, _FMT_COUNT_ARGS(__VA_ARGS__), (format_arg_t[]){_FMT_ARGS_MAP(__VA_ARGS__)})

#define print_to(stream, fmt, ...) _FMT_SELECT_HELPER(dummy, ##__VA_ARGS__,                                                           \
                                                      _print_to_has_args, _print_to_has_args, _print_to_has_args, _print_to_has_args, \
                                                      _print_to_has_args, _print_to_has_args, _print_to_has_args, _print_to_has_args, \
                                                      _print_to_has_args, _print_to_has_args, _print_to_has_args, _print_to_has_args, \
                                                      _print_to_has_args, _print_to_has_args, _print_to_has_args, _print_to_no_args)(stream, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // CRACK_PRINT_H
