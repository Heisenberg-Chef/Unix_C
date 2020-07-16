/*
多了一种申请内存的方式
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define MEMSIZE 1024

int main()
{
    char * ptr;
    pid_t pid;

    ptr = mmap(NULL,MEMSIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);
    }

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        munmap(ptr,MEMSIZE);
        exit(1);
    }

    if(pid == 0)
    {
        //write
        strcpy(ptr,"Hello");
        munmap(ptr,MEMSIZE);
        exit(0);
    }
    else{
        //read
        wait(NULL);//收尸
        puts(ptr);
        munmap(ptr,MEMSIZE);
        exit(0);
    }

    exit(0);
}