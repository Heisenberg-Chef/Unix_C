#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Before while()");
    fflush(NULL);
    while(1)
    ;
    printf("After while()");
    return 0;
}