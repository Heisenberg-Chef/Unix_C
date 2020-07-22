#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>

#define PROCNUM 20
#define FNAME "./num.txt"
#define LINESIZE 1024

static int semid;
//  定义资源量,也是可以进行锁操作,进程之前可以进行通信
static void P()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    while(semop(semid,&op,1)<0)
    {
        if(errno != EINTR || errno != EAGAIN)
        {
            exit(1);
        }
    }
}

static void V()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    while(semop(semid,&op,1)<0)
    {
        if(errno != EINTR || errno != EAGAIN)
        {
            exit(1);
        }
    }
}



static void func_add(void)
{
    FILE * fp;
    char linebuf[LINESIZE];

    fp = fopen(FNAME,"r+");
    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    //这里用的是fopen 是FILE * 的指针但是我们需要一个文件描述符因此
    int fd = fileno(fp);

    P();
    fgets(linebuf,LINESIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);   // 放大故障
    fprintf(fp,"%d\n",atoi(linebuf)+1);
    fflush(fp);
    V();

    fclose(fp);
    return;
}

int main()
{
    int i,err;
    pid_t pid;
    //key = ftok();

    semid = semget(IPC_PRIVATE,1,0666);                   //私有的匿名IPC,flag要写上创建的权限
    if(semid < 0)
    {
        perror("semget()");
        exit(1);
    }
    semctl(semid,0,SETVAL,1);

    for(i = 0;i < PROCNUM;i++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        if(pid == 0)
        {
            func_add();
            exit(0);
        }
    }

    for(i = 0;i < PROCNUM ; i++)
    {
        wait(NULL);
    }
    semctl(semid,0,IPC_RMID,1);
    exit(0);
}