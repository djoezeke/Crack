//////////////////////////////////////////////////////////////////////////////
/// \file memory.c
///
///
/// SECTIONS: Index of this file
///
/// [SECTION] Include Mess
/// [SECTION] Macro Defines
///
/// Internal Declarations
///
///     [SECTION] Internal : Forwards
///     [SECTION] Internal : Structures
///     [SECTION] Internal : Functions
///
/// Public Declarations
///
///     [SECTION] Public : Functions
///
//////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <crack/memory.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Defines
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#pragma region Private

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Forwards
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Structures
    //-----------------------------------------------------------------------------

    typedef struct
    {
        destructor_t destructor;
    } crack_unique_control_t;

    typedef struct
    {
        size_t ref_count;
        destructor_t destructor;
    } crack_shared_control_t;

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Functions
    //-----------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////////
    /// Function Declarations
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    /// Function Definations
    //////////////////////////////////////////////////////////////////////////////

    void crack_unique_cleanup(void *ptr)
    {
        void **real_ptr = (void **)ptr;
        if (real_ptr && *real_ptr)
        {
            crack_unique_release(*real_ptr);
            *real_ptr = NULL;
        }
    }

    void crack_shared_cleanup(void *ptr)
    {
        void **real_ptr = (void **)ptr;
        if (real_ptr && *real_ptr)
        {
            crack_shared_release(*real_ptr);
            *real_ptr = NULL;
        }
    }

    void *crack_unique_create(size_t size, destructor_t dtor)
    {
        crack_unique_control_t *control = (crack_unique_control_t *)calloc(1, sizeof(crack_unique_control_t) + size);
        if (!control)
        {
            return NULL;
        }
        control->destructor = dtor;
        return (void *)(control + 1);
    }

    void crack_unique_release(void *ptr)
    {
        if (!ptr)
            return;
        crack_unique_control_t *control = (crack_unique_control_t *)ptr - 1;
        if (control->destructor)
        {
            control->destructor(ptr);
        }
        free(control);
    }

    void *crack_shared_create(size_t size)
    {
        crack_shared_control_t *control = (crack_shared_control_t *)calloc(1, sizeof(crack_shared_control_t) + size);
        if (!control)
        {
            return NULL;
        }
        control->ref_count = 1;
        control->destructor = NULL;
        return (void *)(control + 1);
    }

    void crack_shared_release(void *ptr)
    {
        if (!ptr)
            return;
        crack_shared_control_t *control = (crack_shared_control_t *)ptr - 1;
        if (--control->ref_count == 0)
        {
            if (control->destructor)
            {
                control->destructor(ptr);
            }
            free(control);
        }
    }

#pragma endregion // Private

#pragma region Public

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Functions
    //-----------------------------------------------------------------------------

    void *crack_unique_move(void *ptr_ref)
    {
        if (!ptr_ref)
            return NULL;
        void **real_ref = (void **)ptr_ref;
        void *temp = *real_ref;
        *real_ref = NULL;
        return temp;
    }

    void *crack_shared_copy(void *ptr)
    {
        if (ptr)
        {
            crack_shared_control_t *control = (crack_shared_control_t *)ptr - 1;
            control->ref_count++;
        }
        return ptr;
    }

#pragma endregion // Public

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
