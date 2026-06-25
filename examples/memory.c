#include <crack/memory.h>
#include <crack/print.h>
#include <stdio.h>

static void dtor(void *ptr)
{
    print("dtor called on {}\n", ptr);
}

int main(void)
{
    // Shared Pointer usage
    {
        shared_ptr int *a = make_shared(int, 23);
        shared_ptr int *b = copy_shared(a); // Increments reference count

        print("a ({}) = {}\n", a, *a);
        print("b ({}) = {}\n", b, *b);

        print("\n");
    } // a and b automatically released here when refcount drops to 0

    // Unique Pointer usage
    {
        unique_ptr int *a = make_unique(int, 23);
        unique_ptr int *b = make_unique(int, 22, dtor); // With custom destructor

        // Array support
        unique_ptr int *d = make_unique(int[5], 22, {1, 22, 3, 4, 5});

        print("a ({}) = {}\n", a, *a);
        print("b ({}) = {}\n", b, *b);

        printf("\n");

        print("d ({}) = d : {}\n", d, *d);

        for (int i = 0; i < 5; i++)
        {
            print("d ({}) = d : {}\n", d, *(d + i));
        }

    } // b's custom destructor and memory automatically cleaned up here

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// LICENSE: Public Domain (www.unlicense.org)
///
/// Copyright (c) 2026 Sackey Ezekiel Etrue
///
/// This is free and unencumbered software released into the public domain.
/// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
/// software, either in source code form or as a compiled binary, for any purpose,
/// commercial or non-commercial, and by any means.
///
/// In jurisdictions that recognize copyright laws, the author or authors of this
/// software dedicate any and all copyright interest in the software to the public
/// domain. We make this dedication for the benefit of the public at large and to
/// the detriment of our heirs and successors. We intend this dedication to be an
/// overt act of relinquishment in perpetuity of all present and future rights to
/// this software under copyright law.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
/// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
/// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///
/// For more information, please refer to <http://unlicense.org/>
//////////////////////////////////////////////////////////////////////////////
