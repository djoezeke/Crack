#ifndef CRACK_SSTREAM_H
#define CRACK_SSTREAM_H

#include <crack/string.h>

typedef char char_type;
typedef string string_type;

typedef string sstream;
typedef string isstream;
typedef string osstream;

typedef enum
{
    in,
    out,
} openmode;

typedef struct streambuf
{
    char_type *beg;
    char_type *end;
    char_type *cur;

} streambuf;

typedef struct
{
    streambuf streambuf;
    string_type string;
    openmode openmode;

} stringbuf;

typedef struct
{
    stringbuf stringbuf;
} isstream;

typedef struct
{
    stringbuf stringbuf;
} osstream;

#endif // CRACK_SSTREAM_H
