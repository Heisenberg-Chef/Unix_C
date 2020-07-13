#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "mytbf.h"

#define CPS 10
#define BUFSIZE 1024
#define BURST 100

int main(int argc,char ** argv)
{
    int sfd,dfd = 1;                    // file description
    char buf[BUFSIZE];
    int size,len,ret,pos=0; // 读取长度
    mytbf_t * tbf;
    umask(0000);
    if(argc<2)
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

    tbf = mytbf_init(CPS,BURST);
    if(tbf == NULL)
    {
        fprintf(stderr,"mytbf_init() failed\n");
        exit(1);
    }
    while(1)
    {
        size = mytbf_fetchtoken(tbf,BUFSIZE);
        while((len = read(sfd,buf,size)) < 0)    // 0
        {
            if(errno == EINTR)
                continue;   //假错误 跳转到 0
            perror("read():");
            break;
        }
        if(len == 0)
        {
            break;
        }
        if(size - len > 0)
            mytbf_returntoken(tbf,size - len);
        pos = 0;
        while(len >0)   // 1
        {
            ret = write(dfd,buf,len);
            //printf("[%d--->%s]\n",ret,buf);
            if(ret < 0)
            {
                if(errno == EINTR)
                    continue;   //跳到1
                perror("write()");
                break;
            }
            pos += ret;
            len -=ret;
        }

    }
    close(sfd);
    mytbf_destory(tbf);
    exit(0);
}
/*  这是一个漏桶实例
在实际的使用中比较僵硬
*/