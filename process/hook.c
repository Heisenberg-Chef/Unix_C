#include <stdio.h>
#include <stdlib.h>

static void f1(void)
{
    puts("f1() is working.\n");
}

static void f2(void)
{
    puts("f2() is working.\n");
}

static void f3(void)
{
    puts("f3() is working.\n");
}

int main()
{
    puts("Begin\n");
    //  并不是调用,而是把这个进程挂在钩子上,在结束之前调用
    atexit(f1);
    atexit(f2);
    atexit(f3);

    puts("End\n");
}