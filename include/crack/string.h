#ifndef CRACK_STRING_H
#define CRACK_STRING_H

#define basic_string(name, type) \
    typedef struct name          \
    {                            \
        type *data;              \
        size_type size;          \
        cap_type cap;            \
    } name;

typedef int size_type;
typedef int cap_type;

basic_string(string, char);
basic_string(wstring, char);

basic_string(u8string, char);
basic_string(u16string, char);
basic_string(u32string, char);

#endif // CRACK_STRING_H
