#include <crack/format.h>
#include <crack/print.h>
#include <crack/vector.h>

typedef struct Product
{
    char *name;
    double price;
} Product;

int main()
{
    // =========================================================================
    // 1. INITIALIZATION & CLASS USAGE
    // =========================================================================
    // Creating a vector that holds instances of our custom class
    // vector(Product) inventory;

    // =========================================================================
    // 2. MODIFIERS & EMPLACE (Adding elements)
    // =========================================================================
    // push_back copies or moves an existing object into the vector
    Product p1 = {"Laptop", 999.99};
    vector_push_back(Product, vec, p1);

    // emplace_back constructs the object in-place, avoiding extra copies
    vector_emplace_back(Product, vec, {"Mouse", 25.50});
    vector_emplace_back(Product, vec, {"Keyboard", 75.50});

    // insert: Add an item at a specific position using an iterator
    inventory.insert(inventory.begin() + 1, Product("Monitor", 299.99));

    // emplace: Construct an item in-place at a specific position
    inventory.emplace(inventory.begin() + 2, "Pad", 10.00);

    // =========================================================================
    // 3. ELEMENT ACCESS
    // =========================================================================
    print("--- Element Access ---");
    // operator[]: Direct index access (No bounds checking)
    // print("Index 0: {}", inventory[0].name);

    // at(): Safe index access (Throws out_of_range if out of bounds)
    print("Index 1: {}", inventory.at(1).name);

    // front() & back(): Fast references to first and last items
    print("First Item: {}", vector_front(Product, vec).name);
    print("Last Item: {}", vector_back(Product, vec).name);

    // data(): Returns a direct pointer to the underlying contiguous array memory
    Product *rawArray = vector_data(Product, vec);
    print("Raw Array Access Pointer (Index 3): ", rawArray[3].name);

    // =========================================================================
    // 4. ITERATORS (Traversal Examples)
    // =========================================================================
    print("\n--- Iterator Traversal ---");
    // Standard forward iterator (begin to end)
    // for (vector<Product>::iterator it = inventory.begin(); it != inventory.end(); ++it)
    // {
    //     print("-> ", it->name, ": $", it->price);
    // }

    // Constant reverse iterator (crbegin to crend) - Read-only backward traversal
    print("\n--- Reverse Constant Traversal ---");
    // for (auto rit = inventory.crbegin(); rit != inventory.crend(); ++rit)
    // {
    //     print("<- ", rit->name);
    // }

    // =========================================================================
    // 5. CAPACITY FUNCTIONS
    // =========================================================================
    print("\n--- Capacity & Structural Info ---");
    print("Is empty? {}", (inventory.empty() ? "Yes" : "No"));
    print("Current Size (Elements count): {}", inventory.size());
    print("Current Capacity (Allocated space): {}", inventory.capacity());
    print("Max Allowable Size: {}", inventory.max_size());

    // reserve(): Request memory expansion ahead of time to avoid reallocations
    inventory.reserve(20);
    print("Capacity after reserve(20): {}", inventory.capacity());

    // shrink_to_fit(): Requests reduction of capacity to fit the current size
    inventory.shrink_to_fit();
    print("Capacity after shrink_to_fit(): {}", inventory.capacity());

    // resize(): Changes the actual number of elements
    inventory.resize(3); // Drops everything after the 3rd element
    print("Size after resize(3): {}", inventory.size());

    // =========================================================================
    // 6. REMOVING ELEMENTS
    // =========================================================================
    // pop_back(): Removes the very last element
    inventory.pop_back();

    // erase(): Removes a specific element using an iterator (Removes 2nd element)
    inventory.erase(inventory.begin() + 1);

    // =========================================================================
    // 7. ASSIGN, SWAP & CLEAR
    // =========================================================================
    vector(Product) backupVec;
    // assign(): Replaces contents with 2 default-constructed Products
    backupVec.assign(2, Product("Default", 0.0));

    // swap(): Exchanges the contents of two vectors instantly (O(1) complexity)
    inventory.swap(backupVec);

    // clear(): Erases all remaining elements, reducing size to 0
    inventory.clear();
    print("\nFinal Inventory Size after clear(): {}", inventory.size());

    // =========================================================================
    // 8. ALLOCATORS
    // =========================================================================
    // get_allocator(): Obtains a copy of the allocator object used to build the vector
    // vector<Product>::allocator_type myAlloc = inventory.get_allocator();

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
