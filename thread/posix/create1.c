#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void *func(void * p)
{
    puts("Thread is working!\n");
    // return NULL;
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    
    int err;

    puts("Begin!\n");
    
    err = pthread_create(&tid,NULL,func,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create(): %s\n",strerror(err));
        exit(1);
    }
    pthread_join(tid,NULL);
    puts("End!\n");
    exit(0);
}



