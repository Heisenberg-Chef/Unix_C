#include "proto.h"

int main()
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in raddr;


    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd < 0 )
    {
        perror("socket()");
        exit(1);
    }
    //  设置socket
    int val = 1;
    if(setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val)))
    {
        perror("setsockopt();");
        exit(1);
    }

    strcpy((char *)sbuf.name,"Ray");
    sbuf.math = htonl(rand()%100);
    sbuf.chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi((RCVPROT))); // 转换 通过网络发射出去
    inet_pton(sd,"255.255.255.255",&raddr.sin_addr.s_addr);
    //bind
    if(sendto(sd,(void*)&sbuf,sizeof(sbuf),MSG_DONTROUTE,(void*)&raddr,sizeof(raddr))<0)   // datagram  send()---stream
    {
        perror("sendto()");
        exit(1);
    }
    puts("ok");

    close(sd);



    exit(0);
}