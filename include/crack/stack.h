//////////////////////////////////////////////////////////////////////////////
/// \file <crack/stack.h>
///
//////////////////////////////////////////////////////////////////////////////

#ifndef CRACK_STACK_H

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

#define CRACK_STACK_H

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Definitions
//-----------------------------------------------------------------------------

#define stack_init(type, stack, size)                           \
    do                                                          \
    {                                                           \
        (stack)->start = (type *)malloc((size) * sizeof(type)); \
        if ((stack)->start == NULL)                             \
        {                                                       \
            perror("Failed to allocate memory");                \
            exit(EXIT_FAILURE);                                 \
        }                                                       \
        (stack)->top = (stack)->start;                          \
        (stack)->end = (stack)->start + (size);                 \
    } while (0);

//////////////////////////////////////////////////////////////////////////////
/// \brief stack clear
//////////////////////////////////////////////////////////////////////////////
#define stack_clear(type, stack) ((stack)->top = (stack)->start)

#define stack_empty(type, stack) (((stack)->start) == ((stack)->top))

#define stack_size(type, stack) (((stack)->top) - ((stack)->start))

#define stack_push(type, stack, value)                                                      \
    do                                                                                      \
    {                                                                                       \
        if ((stack)->top >= (stack)->end)                                                   \
        {                                                                                   \
            size_t current_capacity = (stack)->end - (stack)->start;                        \
            size_t new_capacity = current_capacity * 2;                                     \
            size_t current_size = (stack)->top - (stack)->start;                            \
                                                                                            \
            type *new_start = (type *)realloc((stack)->start, new_capacity * sizeof(type)); \
            if (new_start == NULL)                                                          \
            {                                                                               \
                perror("Failed to grow memory");                                            \
                exit(EXIT_FAILURE);                                                         \
            }                                                                               \
                                                                                            \
            (stack)->start = new_start;                                                     \
            (stack)->top = (stack)->start + current_size;                                   \
            (stack)->end = (stack)->start + new_capacity;                                   \
        }                                                                                   \
        *((stack)->top) = (value);                                                          \
        (stack)->top++;                                                                     \
    } while (0);

#define stack_peek(type, stack) (((!stack_empty(type, stack)) ? (*((stack)->top - 1)) : (0)))
#define stack_top(type, stack) (((!stack_empty(type, stack)) ? (*((stack)->top - 1)) : (0)))

#define stack_emplace(type, stack, value)                                                   \
    do                                                                                      \
    {                                                                                       \
        if ((stack)->top >= (stack)->end)                                                   \
        {                                                                                   \
            size_t current_capacity = (stack)->end - (stack)->start;                        \
            size_t new_capacity = current_capacity * 2;                                     \
            size_t current_size = (stack)->top - (stack)->start;                            \
                                                                                            \
            type *new_start = (type *)realloc((stack)->start, new_capacity * sizeof(type)); \
            if (new_start == NULL)                                                          \
            {                                                                               \
                perror("Failed to grow memory");                                            \
                exit(EXIT_FAILURE);                                                         \
            }                                                                               \
                                                                                            \
            (stack)->start = new_start;                                                     \
            (stack)->top = (stack)->start + current_size;                                   \
            (stack)->end = (stack)->start + new_capacity;                                   \
        }                                                                                   \
        *((stack)->top) = (value);                                                          \
        (stack)->top++;                                                                     \
    } while (0);

#define stack_pop(type, stack)

#define stack_free(type, stack)                              \
    do                                                       \
    {                                                        \
        free((stack)->start);                                \
        (stack)->start = (stack)->top = (stack)->end = NULL; \
    } while (0);

#define stack_swap(type, stack_a, stack_b)

#define stack_type(type) \
    typedef struct       \
    {                    \
        type *start;     \
        type *end;       \
        type *top;       \
    } stack##type;

#define stack(type) stack##type

//-----------------------------------------------------------------------------
// [SECTION] Internal Declarations
//-----------------------------------------------------------------------------

#pragma region Private

//-----------------------------------------------------------------------------
// [SECTION] Internal : Macros
//-----------------------------------------------------------------------------

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

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Functions
    //-----------------------------------------------------------------------------

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma endregion // Private

//-----------------------------------------------------------------------------
// [SECTION] Public Declarations
//-----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
/// \defgroup stack Stack
/// \ingroup sequences
///
/// A standard container giving FILO behavior.
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

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Structures
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Functions
    //-----------------------------------------------------------------------------

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma endregion // Public

/// \} group stack

//-----------------------------------------------------------------------------
// [SECTION] Internal Cleanups
//-----------------------------------------------------------------------------

#endif // CRACK_STACK_H
