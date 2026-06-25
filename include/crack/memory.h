//////////////////////////////////////////////////////////////////////////////
/// \file <crack/memory.h>
///
//////////////////////////////////////////////////////////////////////////////

#ifndef CRACK_MEMORY_H

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

#define CRACK_MEMORY_H

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Definitions
//-----------------------------------------------------------------------------

// Smart pointer decorators
#if defined(__GNUC__) || defined(__clang__)
#define shared_ptr __attribute__((cleanup(crack_shared_cleanup)))
#define unique_ptr __attribute__((cleanup(crack_unique_cleanup)))
#else
#define shared_ptr
#define unique_ptr
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef void (*destructor_t)(void *);

    // Cleanup functions used by GCC/Clang cleanup attributes
    void crack_shared_cleanup(void *ptr);
    void crack_unique_cleanup(void *ptr);

    // Core unique_ptr functions
    void *crack_unique_create(size_t size, destructor_t dtor);
    void crack_unique_release(void *ptr);

    // Core shared_ptr functions
    void *crack_shared_create(size_t size);
    void *crack_shared_copy(void *ptr);
    void crack_shared_release(void *ptr);

    // Helper functions for portable initialization and moving

    static inline void *crack_init_helper(void *ptr, const void *val, size_t size)
    {
        if (ptr && val)
        {
            memcpy(ptr, val, size);
        }
        return ptr;
    }

    static inline void *crack_move_helper(void *ptr_ref);

// Helper macros for make_unique
#define crack_make_unique_1(type, init) \
    crack_init_helper(crack_unique_create(sizeof(type), NULL), &(type){init}, sizeof(type))

#define crack_make_unique_2(type, init, dtor) \
    crack_init_helper(crack_unique_create(sizeof(type), (destructor_t)(dtor)), &(type){init}, sizeof(type))

#define crack_make_unique_many(type, val, ...) \
    crack_init_helper(crack_unique_create(sizeof(type), NULL), &(type)__VA_ARGS__, sizeof(type))

// Macros to select make_unique implementation based on argument count
#define CRACK_GET_UNIQUE_MACRO(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define make_unique(type, ...) \
    CRACK_GET_UNIQUE_MACRO(__VA_ARGS__, crack_make_unique_many, crack_make_unique_many, crack_make_unique_many, crack_make_unique_many, crack_make_unique_many, crack_make_unique_2, crack_make_unique_1)(type, __VA_ARGS__)

// Macros for shared_ptr
#define make_shared(type, init) \
    crack_init_helper(crack_shared_create(sizeof(type)), &(type){init}, sizeof(type))

#define copy_shared(sptr) crack_shared_copy(sptr)
#define move_unique(uptr) crack_move_helper(&(uptr))

#define get_unique(uptr) (uptr)
#define get_shared(sptr) (sptr)

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // CRACK_MEMORY_H
