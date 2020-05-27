#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    sfd = open(argv[1],O_RDONLY);
    if(sfd<0)
    {
        perror("open(1):");
        exit(1);
    }
    dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);       // IMPORTANT

    if(dfd<0)
    {
        close(sfd);
        perror("open(2)");
        exit(1);
    }

    while(1)
    {
        len = read(sfd,buf,BUFSIZE);
        if(len < 0)
        {
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