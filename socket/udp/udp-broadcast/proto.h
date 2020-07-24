#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
/*
将需要的头文件包括在photo.h里面;
*/
#ifndef PROTO_H__
#define PROTO_H__

#define NAMESIZE    11      // 选一个,一定不对齐的长度
#define RCVPROT     "1989"    // 选1024以上的端口,到时候用的时候使用atoi转换一下

struct msg_st
{
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));  // 不对齐

#endif