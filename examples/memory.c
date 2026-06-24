#include <crack/memory.h>
#include <stdio.h>

static void dtor(void *ptr)
{
    printf("dtor called on %p\n", ptr);
}

static int *increment(int count)
{
    shared_ptr int *a = make_shared(int, 0);
    *a = *a + count;
    printf("c (%p) = %d\n", a, *a);
    return a;
}

int main(void)
{
    // Shared Pointer usage
    {
        shared_ptr int *a = make_shared(int, 23);
        shared_ptr int *b = copy_shared(a); // Increments reference count

        printf("a (%p) = %d\n", a, *a);
        printf("b (%p) = %d\n", b, *b);

        printf("\n");

        int *c = increment(10);

        printf("c (%p) = %d\n", c, *c);

        printf("\n");
    } // a and b automatically released here when refcount drops to 0

    // Unique Pointer usage
    {
        unique_ptr int *a = make_unique(int, 23);
        unique_ptr int *b = make_unique(int, 22, dtor); // With custom destructor

        // Array support
        unique_ptr int *d = make_unique(int[5], 22, {1, 22, 3, 4, 5});

        printf("a (%p) = %d\n", a, *a);
        printf("b (%p) = %d\n", b, *b);

        printf("\n");

        printf("d (%p) = d : %d\n", d, *d);

        for (int i = 0; i < 5; i++)
        {
            printf("d (%p) = d : %d\n", d, *(d + i));
        }

    } // b's custom destructor and memory automatically cleaned up here

    return 0;
}
