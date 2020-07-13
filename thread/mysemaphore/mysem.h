#ifndef MYSEM_H__
#define MYSEM_H__
#include <stdio.h>
#include <stdlib.h>

typedef void mysem_t;

mysem_t * mysem_init(int initval);
int mysem_add(mysem_t * , int); //归还
int mysem_sub(mysem_t *,int );  //取资源
int mysem_destroy(mysem_t *);

#endif