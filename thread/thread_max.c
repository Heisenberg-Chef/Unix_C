#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static void * func(void * p)
{
    while(1)
        pause();
    pthread_exit(NULL);
}

int main()
{
    int i,err=0;
    pthread_t tid;
    for(i=0;;i++)
    {
        pthread_create(&tid,NULL,func,NULL);
        if(err)
        {
            printf("err.\n");
            break;
        }
    }

    

    printf("%d\n",i);
}