#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define THRNUM 20
#define FNAME "./out"
#define LINESIZE 4096

//  添加一个互斥量,肯定是全局的定义
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void * thr_add(void * p)
{
    FILE * fp;
    fp = fopen(FNAME,"r+");
    char linebuf[LINESIZE];

    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    pthread_mutex_lock(&mut);          // 进入临界区
    fgets(linebuf,LINESIZE,fp);    // buffer弄大一些.
    //sleep(1);
    fseek(fp,0,SEEK_SET);

    fprintf(fp,"%d\n",atoi(linebuf)+1);
    fclose(fp);
    pthread_mutex_unlock(&mut); //走出临界区
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid[THRNUM];
    int err;

    //创建线程环节
    for(int i = 0;i < THRNUM;i++)
    {
        err = pthread_create(tid + i,NULL,thr_add,NULL);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }


    //回收线程环节
    for(int i = 0;i<THRNUM;i++)
    {
         pthread_join(tid[i],NULL);
    }


    pthread_mutex_destroy(&mut);
    exit(0);
}