#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PROCNUM 20
#define FNAME "./num.txt"
#define LINESIZE 1024

 
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

    lockf(fd,F_LOCK,0);
    fgets(linebuf,LINESIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);   // 放大故障
    fprintf(fp,"%d\n",atoi(linebuf)+1);
    fflush(fp);
    lockf(fd,F_ULOCK,0);

    fclose(fp);
    return;
}

int main()
{
    int i,err;
    int pid;
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
    
    exit(0);
}