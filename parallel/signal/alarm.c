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
    signal(SIGALRM,int_handler); // 1.程序没结束 2.信号来了 进行执行.
    alarm(5);   // 5秒后触发闹钟信号,该信号默认是杀死当前进程.  不能
    for(i = 0;i < 10; i++)
    {
        write(1,"*",1);
        sleep(1);
    }
    exit(0);
}