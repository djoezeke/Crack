#include <crack/memory.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct
    {
        destructor_t destructor;
    } crack_unique_control_t;

    typedef struct
    {
        size_t ref_count;
        destructor_t destructor;
    } crack_shared_control_t;

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

    void *crack_shared_copy(void *ptr)
    {
        if (ptr)
        {
            crack_shared_control_t *control = (crack_shared_control_t *)ptr - 1;
            control->ref_count++;
        }
        return ptr;
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

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
