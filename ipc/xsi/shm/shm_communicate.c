#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define MEMSIZE 1024

static int shmid;

int main()
{
    pid_t pid;
    char * ptr;

    shmid = shmget(IPC_PRIVATE,MEMSIZE,0660);
    if(shmid < 0)
    {
        perror("shmget()");
        exit(1);
    }

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)
    {
        ptr = shmat(shmid,NULL,0);//创建空间映射
        strcpy(ptr,"HELLO\n");
        shmdt(ptr);//删除空间映射
        exit(0);
    }
    else{
        ptr = shmat(shmid,NULL,0);
        wait(NULL);             //  如果不等待 打印不出来,因为有可能子进程还没写ptr空间的数据
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid,IPC_RMID,NULL);
    }
    //free(ptr);


    exit(0);
}