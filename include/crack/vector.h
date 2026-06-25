//////////////////////////////////////////////////////////////////////////////
/// \file <crack/vector.h>
///
//////////////////////////////////////////////////////////////////////////////

#ifndef CRACK_VECTOR_H

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

#define CRACK_VECTOR_H

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Definitions
//-----------------------------------------------------------------------------

#define vector(type)
#define vector_push_back(type, vector, value)
#define vector_emplace_back(type, vector, value)
#define vector_insert(type, vector, pos, value)
#define vector_emplace(type, vector, pos, value)
#define vector_at(type, vector, pos)
#define vector_front(type, vector)
#define vector_back(type, vector)
#define vector_data(type, vector)
#define vector_empty(type, vector)
#define vector_size(type, vector)
#define vector_capacity(type, vector)
#define vector_max_size(type, vector)
#define vector_pop_back(type, vector)
#define vector_erase(type, vector, pos)
#define vector_reserve(type, vector, value)
#define vector_resize(type, vector, value)
#define vector_shrink_to_fit(type, vector)
#define vector_assign(type, vector, pos, value)
#define vector_clear(type, vector)
#define vector_swap(type, vector_a, vector_b)

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
/// \defgroup vector Vector
///
/// Support for vector.
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

/// \} group vector

//-----------------------------------------------------------------------------
// [SECTION] Internal Cleanups
//-----------------------------------------------------------------------------

#endif // CRACK_VECTOR_H
