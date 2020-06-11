#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

static void int_handler(int s)
{
    write(1,"!",1);
}


int main()
{

    int i ;
    //  signal(SIGINT,SIG_IGN); // 忽略了这个信号
    signal(SIGINT,int_handler); // 1.程序没结束 2.信号来了 进行执行.
    for(i = 0;i < 10; i++)
    {
        write(1,"*",1);
        sleep(1);
    }
    exit(0);
}