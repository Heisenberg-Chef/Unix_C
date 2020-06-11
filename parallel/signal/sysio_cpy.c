#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFSIZE 10

int main(int argc,char ** argv)
{
    int sfd,dfd;                    // file description
    char buf[BUFSIZE];
    int len,ret,pos=0; // 读取长度

    umask(0000);
    if(argc<3)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    do{
        sfd = open(argv[1],O_RDONLY);
        if(sfd<0)
        {
            if(errno != EINTR)
            {
                perror("open(1):");
                exit(1);
            }
        }
    }while(sfd<0);
    do
    {
        /* code */
            
        dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);       // IMPORTANT
        if(dfd<0)
        {
            if(errno != EINTR)  // 信号:在我们打开一个很慢的设备的时候可能有信号中断
            {                   // 我们当前的操作,所以open函数会引发EINTR的异常,
                close(sfd);     // 我们可以理解为假错误.
                perror("open(2)");
                exit(1);
            }
        }
    } while (dfd < 0);


    while(1)
    {
        len = read(sfd,buf,BUFSIZE);
        if(len < 0)
        {
            if(errno == EINTR)
                continue;
            perror("read():");
            break;
        }
        if(len == 0)
        {
            break;
        }
        while(len >0)
        {
            ret = write(dfd,buf,len);
            printf("[%d--->%s]\n",ret,buf);
            if(ret < 0)
            {
                if(errno == EINTR)
                    continue;
                perror("write()");
                break;
            }
            pos += ret;
            len -=ret;
        }
    }
    close(sfd);
    close(dfd);
    exit(0);
}