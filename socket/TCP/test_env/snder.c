#include "proto.h"

int main()
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in client_addr;


    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    strcpy((char *)sbuf.name,"Ray");
    sbuf.math = htonl(rand()%100);
    sbuf.chinese = htonl(rand()%100);

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(RCVPROT); // 转换 通过网络发射出去
    inet_pton(sd,"140.143.223.179",&client_addr.sin_addr.s_addr);
    connect(sd,(void*)&client_addr,sizeof(client_addr));
    if(send(sd,&sbuf,sizeof(sbuf),0)<0)   // datagram  send()---stream
    {
        perror("send()");
        exit(1);
    }
    puts("ok");
    close(sd);
    exit(0);
}
