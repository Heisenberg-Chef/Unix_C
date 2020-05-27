#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define FNAME "./out"

int main()
{
    int fd,tmp;

    // close(1);
    //关闭了标准输出
    fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0644);//按照最小分配原则1号输出被我现在的文件填上了.
    if(fd < 0)
    {
        perror("open():");
        exit(1);
    }
    dup2(fd,1);//是下面2句话的原子操作
    // close(1);
    // dup(fd);
    if(fd != 1)
    {
        close(fd);
    }
    //*******************************
    puts("hello!");

    exit(0);
}