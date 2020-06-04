#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t   pid;
    printf("[%d]Begin!\n",getpid());
    pid = fork();
    if(pid < 0)
    {
        //如果成功在父进程返回id号,在子进程中返回0
        perror("fork()");
        exit(1);
    }
    if(pid == 0 ) //child
    {
        printf("[%d]:Child is working.\n",getpid());
    }else{  // parent
        getchar();
        printf("[%d]:Parent is working.\n",getpid());
    }



    printf("[%d]End...\n",getpid());



    exit(0);
}