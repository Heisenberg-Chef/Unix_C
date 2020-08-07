#include "proto.h"

int main()
{
    int sd;
    struct sockaddr_in server_addr;
    struct msg_st rbuf;

    sd = socket(PF_INET,SOCK_STREAM,0);
    inet_pton(AF_INET,"0.0.0.0",&server_addr.sin_addr.s_addr);
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(RCVPROT);
    if(bind(sd,(void *)&server_addr,sizeof(server_addr)))
    {
        perror("bind()");
        exit(1);
    }
    listen(sd,1);
    unsigned int len = sizeof(struct sockaddr);
    struct sockaddr tmp;
    int connsd;
    while(1)
    {
        connsd = accept(sd,&tmp,&len);
        if(recv(connsd,&rbuf,sizeof(rbuf),0) == sizeof(rbuf))
        {
            printf("----- MESSAGE GET : -----\n");
            printf("NAME = %s\n",rbuf.name);
            printf("MATH = %d\n",ntohl(rbuf.math));
            printf("CHINESE = %d\n",ntohl(rbuf.chinese));
        }
    }

    exit(0);
}