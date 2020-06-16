#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>

#define CPS 1          // charactors per second.
#define BUFSIZE CPS 

static volatile int loop = 1;

static void clock_handler(int interupt)
{
    // alarm(1);  //    使用setitimer可以省去循环调用的操作,在kernal中进行了封装
    loop = 0;
}
int main(int argc,char ** argv)
{
    int sfd,dfd = 1;                    // file description
    char buf[BUFSIZE];
    int len,ret,pos=0; // 读取长度
    struct itimerval it;


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

    signal(SIGALRM,clock_handler);
    //  alarm(1);
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 4000;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 4000;
    setitimer(ITIMER_REAL,&it,NULL);

    while(1)
    {
        while(loop)
        {
            pause(); //禁止CPU忙等
        }
        loop = 1;   //出循环马上把标记改为1
        while((len = read(sfd,buf,BUFSIZE)) < 0)    // 0
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
    close(dfd);
    exit(0);
}
/*  这是一个漏桶实例
在实际的使用中比较僵硬
*/