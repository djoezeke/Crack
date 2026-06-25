#include <crack/print.h>
#include <crack/stack.h>

int main()
{
    // 1. Declaration (Uses deque as default underlying container)
    stack_type(int);

    stack(int) st;
    stack_init(int, &st, 2);

    // 2. push() - Insert elements
    stack_push(int, &st, 10);
    stack_push(int, &st, 20);
    stack_push(int, &st, 30); // 30 is now at the top

    // 3. emplace() - Insert element constructed in-place
    stack_emplace(int, &st, 40); // 40 becomes the new top

    // 4. size() - Check current element count
    print("Stack size: {} \n", stack_size(int, &st)); // Output: 4

    // 5. top() - Accessing elements without removing them
    print("Top element: {} \n", stack_top(int, &st)); // Output: 40

    // 6. pop() - Remove top element
    // stack_pop(int, &st);
    // print("New top element after pop: {}\n", stack_top(int, &st)); // Output: 30

    // 7. empty() - Check if stack is empty while clearing it
    // print("Popping remaining elements: ");
    // while (!stack_empty(int, &st))
    // {
    //     print("{} ", stack_top(int, &st));
    //     stack_pop(int, &st);
    // }
    // print("\n"); // Output: 30 20 10

    // 8. swap() - Exchanging contents of two stacks
    // stack(int) stackA;
    // stack(int) stackB;

    // stack_push(int, stackA, 100);
    // stack_push(int, stackB, 500);

    // stack_swap(int, stackA, stackB);
    // print("stackA top after swap: {}\n", stack_top(int, stackA)); // Output: 500

    // 9. Custom underlying container definition
    // stack<int, vector<int>> vectorStack;

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
