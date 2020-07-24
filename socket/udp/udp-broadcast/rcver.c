#include "proto.h"

int main()
{
    int sd;
    struct sockaddr_in laddr,inaddr;
    struct msg_st rbuf;         // 这是我proto定义的....
    socklen_t raddr_len;
    char ipstr[100];

    sd = socket(AF_INET,SOCK_DGRAM,0);  // protocol,使用0 默认的模式 == IPPROTO_UDP
    if(sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPROT));//htons 2个字节
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);// uint32_t 但是是一个数,平时是一个点分式

    if(bind(sd,(void*)&laddr,sizeof(laddr)))
    {
        perror("bind()");
        exit(2);
    }
    //!!!
    raddr_len = sizeof(inaddr);


    while(1)
    {
        recvfrom(sd,&rbuf,sizeof(rbuf),0,(void*)&inaddr,&raddr_len); 
        inet_ntop(AF_INET,&inaddr.sin_addr.s_addr,ipstr,100);
        printf("----- MESSAGE FROM : %s:%d -----\n",ipstr,ntohs(inaddr.sin_port));
        printf("NAME = %s\n",rbuf.name);
        printf("MATH = %d\n",ntohl(rbuf.math));
        printf("CHINESE = %d\n",ntohl(rbuf.chinese));
    }

    close(sd);
    exit(0);
}