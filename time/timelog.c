#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FNAME "../out"
#define BUFSIZE 1024

int main(int argc,char ** argv)
{   
    FILE * fp;
    char buf[BUFSIZE];
    unsigned int count = 0;
    time_t stamp;
    struct tm * tm;
    
    // fp = fopen(FNAME,"a+");
    // if(fp == NULL)
    // {
    //     perror("fopen()");  
    //     exit(1);
    // }
    // while(1)
    // {
    //     if(fgets(buf,BUFSIZE,fp)!=NULL)
    //     {
    //         count++;
    //     }
    //     else{
    //         break;
    //     }
    // }

    while(1)
    {
        stamp = time(NULL);
        tm = localtime(&stamp);
        fprintf(stdout,"%-4d:%d-%d-%d %d:%d:%d\n",++count,\
        tm->tm_year + 1900,tm->tm_mon + 1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
        sleep(1);
    }
    exit(0);
}