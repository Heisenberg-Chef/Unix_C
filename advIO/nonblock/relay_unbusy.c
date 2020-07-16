#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  //  文件描述符 封装
#include <errno.h>
#include <math.h>

#define TTY1 "/dev/ttys002"
#define TTY2 "/dev/ttys003"
#define BUFSIZE 1024

enum state{
    STATE_R = 1,    //读
    STATE_W,        //写
    STATE_Ex,       //其他
    STATE_T         //终止
};

struct fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char * errstr;
};

static void fsm_driver(struct fsm_st * fsm)
{
    int ret;
    //有限状态机
    switch (fsm->state)
    {
    case STATE_R:
        fsm->pos = 0;               
        fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
        if(fsm->len == 0) 
        {
            fsm->state = STATE_T;
        }
        else if(fsm->len < 0)
        {
            if(errno == EAGAIN)
                fsm->state = STATE_R;
            else
                {
                    fsm->errstr="read()";
                    fsm->state = STATE_Ex;
                }
        }
        else
        {
            fsm->pos = 0;
            fsm->state = STATE_W;
        }
        break;
    case STATE_W:
        ret = write(fsm->dfd,(fsm->buf + fsm->pos),fsm->len);
        if(ret < 0)
        {
            if(errno == EAGAIN)
            {
                fsm->state = STATE_W;
            }
            else
            {
                fsm->errstr = "write()"; 
                fsm->state = STATE_Ex;
            }
        }
        else{
            fsm->pos += ret;
            fsm->len -= ret;
            if(fsm->len == 0)
                fsm->state = STATE_R;
            else
            {
                fsm->state = STATE_W;
            }
        }
        break;
    case STATE_Ex:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        break;
    case STATE_T:
        break;
    default:
        abort();
        break;
    }
}

static void relay(int fd1,int fd2)
{
    // 进出我们这个模块前后状态是一样的
    fd_set rset,wset;
    struct fsm_st fsm12,fsm21;
    int fd1_save;
    fd1_save = fcntl(fd1,F_GETFL);
    fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
    int fd2_save;
    fd2_save = fcntl(fd2,F_GETFL);
    fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);

    //  自定义1号状态机
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    //自定义2号状态机
    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    while(fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        // 如果他们都不是T态,那么就一直驱动状态机
        //在这里布置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if(fsm12.state == STATE_R)
            FD_SET(fsm12.sfd,&rset);

        
        //监视
        if(select(max(fd1,fd2)+1,&rset,&wset,NULL,NULL) < 0)    // r w 没有const修饰 不能用while,出现假错误会冲掉设定
        {
            if(errno == EINTR)
                continue;
            perror("select()");
            exit(1);
        }
        //查看监视结果

        //有条件的推动状态机
        if()
            fsm_driver(&fsm12);
        if()    
            fsm_driver(&fsm21);
    }

    // 恢复现场
    fcntl(fd1,F_SETFL,fd1_save);
    fcntl(fd2,F_SETFL,fd2_save);

}

int main()
{
    int fd1,fd2;
    fd1 = open(TTY1,O_RDWR);
    if(fd1<0)
    {
        perror("open(1)");
        exit(1);
    }
    fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
    if(fd2 < 0)
    {
        perror("open(2)");
        exit(1);
    }
    relay(fd1,fd2);

    close(fd1);
    close(fd2);
    exit(0);
}