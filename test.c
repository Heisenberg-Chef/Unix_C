#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

typedef void function_template(void);
typedef void (*function_template2)(void);


void func()
{
    printf("hello world\n");
}

int main()
{
    function_template * a = func;
    a();
    function_template2 b =func;
    b();
    return 0;
}