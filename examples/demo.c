#include <crack/format.h>
#include <crack/print.h>

int main(void)
{
    char *f = format("Hello : {}", "World!");
    println(f);
    print(f, " \n");

    return EXIT_SUCCESS;
}
