#include <crack/print.h>
#include <stdio.h>
#include <stdlib.h>

void print_impl(FILE *stream, const char *fmt, size_t argc, const format_arg_t argv[])
{
    char *s = format_impl(fmt, argc, argv);
    if (s)
    {
        fputs(s, stream);
        free(s);
    }
}

void println_impl(FILE *stream, const char *fmt, size_t argc, const format_arg_t argv[])
{
    char *s = format_impl(fmt, argc, argv);
    if (s)
    {
        fputs(s, stream);
        free(s);
    }
    fputc('\n', stream);
}
