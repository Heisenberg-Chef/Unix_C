#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FNAME "./tmp"

static int daemonize()
{
    pid = fork();

    if(pid < 0) //failed
    {
        perror("fork()");
        return -1;
    }
    if(pid>0) // parent
    {
        return 0;
    }
    fd = open("/dev/null",O_RDWR);
    if(fd < 0 )
    {
        perror("open()");
        return -1;
    }

    setsid();

}

int main()
{
    FILE * fp;
    daemonize();

    fopen(FNAME,"w");
    if(fp == NULL)
    {
        perror("fopen()")
        exit(1);
    }
    for(int i = 0; i < 60 ; i++)
    {
        fprintf(fp,"%d\n",i);
        fflush(fp);
        sleep(1);
    }


    exit(0);
}