#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static void f1(void *p)
{
    printf("f1():%s\n",p);
}

static void f2(void *p)
{
    printf("f2():%s\n",p);
}

static void f3(void *p)
{
    printf("f3():%s\n",p);
}

int main()
{
    puts("Begin!");
    // 5,f1,"aaa"
    // 2,f2,"bbb"
    // 7,f3,"ccc"
    //Begin!End!..bbb...aaa..ccc.........
    puts("End!");
    while(1)
    {
        write(1,". ",1);
        sleep(1);   //  测试中暂时使用sleep
    }
    exit(0);
}