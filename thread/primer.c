#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT - LEFT + 1)

static void * thr_prime(void * p);

struct thr_arg_st
{
    int n;
};

int main()
{
    int i,err;
    pthread_t tid[THRNUM];//把我们常见的TID都保存起来.
    // struct thr_arg_st *p;
    void * ptr;
    for(i = LEFT;i<=RIGHT;i++)
    {   
        int * p = NULL;
        p = malloc(sizeof(int));
        *p = i;
        err = pthread_create(tid+(i - LEFT),NULL,thr_prime,p);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }
    for(i = LEFT;i<=RIGHT;i++)
    {
        pthread_join(tid[i - LEFT],&ptr);
        free(ptr);
    }
    exit(0);
}

static void * thr_prime(void * p)
{
    int j,mark = 1;
    int i = *(int*)p;
    for(j = 2;j<i/2;j++)
    {
        if(i%j == 0)
        {
            mark = 0;
            break;
        }
    }
    if(mark)
    {
        printf("%d is a primer.\n",i);
    }
    // sleep(100);
    pthread_exit(p);
}
