#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 30000000
#define RIGHT 30000200

#define N   3

int main()
{
    pid_t pid;
    int mark,n;

    //创建进程
    for(n = 0;n<N;n++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(1); //如果还有别的进程没来收尸,否则会成为孤儿进程被init接管了 
            /*在这里只不过笼统的处理了毕竟就3个进程,
            以我个人的习惯是做一个pid_t类型的数组来记录他们,他的值是一个static放在静态区里面的
            如果遇到问题全局释放他们.并且引发异常中断.*/
        }
        if(pid ==0)
        {
            for(long i = LEFT+n;i<=RIGHT;i+=N)
            {
                mark = 1;
                for(long j = 2;j<i/2;j++)
                {
                    if(i%j == 0)
                    {
                        mark = 0;
                        break;
                    }
                }
                if(mark)
                {
                    printf("PID[%d]---->[%d]%ld is a primer.\n",getpid(),n,i);
                }
            }
            exit(0);
        }
    }

    for(n = 0;n < N;n++)
        wait(NULL);
    exit(0);
}