#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int main()
{
    pthread_t tid;
    int err;

    puts("Begin!\n");
    err = pthread_create(&tid,NULL,func,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
    }
    
    puts("End...\n");




    exit(0);
}