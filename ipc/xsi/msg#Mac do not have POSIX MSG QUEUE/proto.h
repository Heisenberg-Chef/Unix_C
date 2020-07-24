/*
约定协议:
*/
#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "./services"
#define KEYPROJ 'g'     //  整形数
#define NAMESIZE 1024

struct msg_st{
    char name[NAMESIZE];
    int math;
    int chinese;
};

#endif