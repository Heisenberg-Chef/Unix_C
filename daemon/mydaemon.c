#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

#define FNAME "./tmp"

static char buf[1024];
static time_t timer;
//  返回0为正常结束,其他的为异常
static int daemonize()
{
    pid_t pid;
    int fd;

    pid = fork();

    if(pid < 0) //failed
    {
        return -1;
    }
    if(pid>0) // parent
    {
        //sleep(5);
        exit(0);
    }
    fd = open("/dev/null",O_RDWR);
    if(fd < 0 )
    {
        perror("open()");
        return -1;
    }
    //  重定向
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd > 2)
        close(fd);
    setsid();
    chdir("."); //把当前的工作路径设置到根目录,防止device is busy

    return 0;
}

int main()
{
    FILE * fp;

    openlog("mydaemon",LOG_PID,LOG_DAEMON); //没有返回值一定成功
    if(daemonize())
    {
        syslog(LOG_ERR,"daemonize failed.");    // 千万不能写\n
        exit(1);
    }
    else{
        syslog(LOG_INFO,"daemonize() successed!");
    }

    fp = fopen(FNAME,"w+");
    if(fp == NULL)
    {
        syslog(LOG_ERR,"fopen() failed %s.",strerror(errno));
        exit(1);
    }
    else{
        syslog(LOG_INFO,"fopen() successed.");
    }
    for(int i = 0; i < 60 ; i++)
    {
        timer = time(NULL);
        fprintf(fp,"Atom clock[%ld] date:%s -----> countdown [%d]\n",\
        timer,asctime(localtime(&timer)),i);
        
        fflush(NULL);
        syslog(LOG_DEBUG,"%d is printed.",i);
        sleep(1);
    }
    closelog();
    exit(0);
}