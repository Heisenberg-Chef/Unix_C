/*
我们来实现一个,在没有限制线程总量的情况下,控制同时存在的线程的数量.
不影响调用的前提下,实现一个资源的控制
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "mysem.h"

#define LEFT 1
#define RIGHT 100000
#define THRNUM (RIGHT - LEFT)
#define N 4


static pthread_t tid[THRNUM];
static mysem_t *sem;

static void * fun_thr(void * p)
{
    int i = (int)p;
    int mark = 1;
    for(int j = 2;j < ((j+1)/2);j++)
    {
        if(i%j == 0)
        {
            mark = 0;
            break;
        }
    }
    if(mark == 1)
    {
        printf("[%d] is a primer number.\n",i);
    }
    sleep(1);
    mysem_add(sem,1);
    pthread_exit(NULL);
}


int main()
{ 
    sem = mysem_init(N);
    if(sem == NULL)
    {
        fprintf(stderr,"mysem_init() failed.\n");
        exit(1);
    }
    for(int i = 1;i < (THRNUM + 1);i++)
    {
        mysem_sub(sem,1);
        pthread_create(tid+i,NULL,fun_thr,(void *)i);
    }

    mysem_destroy(sem);
    return 0;
}