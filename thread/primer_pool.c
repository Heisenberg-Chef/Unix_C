#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void * thr_prime(void * p);


int main()
{
    int i,err,arr[THRNUM];
    pthread_t tid[THRNUM];//把我们常见的TID都保存起来.
    for(i = 0;i < THRNUM;i++)
    {   
        arr[i] = i;
        err = pthread_create(tid+i,NULL,thr_prime,arr+i);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }

    for(i=LEFT;i<=RIGHT;i++)
    {
        pthread_mutex_lock(&mut_num);
        while(num != 0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();  // 调度状态颠簸去除,非常短的一个sleep而且不会造成线程的颠簸,
            pthread_mutex_lock(&mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
        //sched_yield();
    }

    pthread_mutex_lock(&mut_num);
    while(num != 0) //  为了防止最后一个结束时候吧其他的任务抢走,我们在这里继续让它等待
    {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);

    for(i = 0;i<=THRNUM;i++)
    {
        pthread_join(tid[i],NULL);
    }

    pthread_mutex_destroy(&mut_num);    //  保证独占,在名字上区分,在我们即将推出main线程的时候,销毁互斥量.
    exit(0);
}

static void * thr_prime(void * p)
{
    int i,j,mark = 1;
    while(1)
    {
        pthread_mutex_lock(&mut_num);

        while(num == 0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }

        if(num == -1)
        {
            pthread_mutex_unlock(&mut_num); //  一定要进行解锁,否则跳转之前会造成死锁
            break;
        }
        i = num;
        num = 0;
        pthread_mutex_unlock(&mut_num);
        //  开始计算
        for(j = 2;j<i/2;j++)
        {
            if(i%j == 0)
            {
                mark = 0;
                break;
            }
        }
        //  计算完成之后会运行一下代码
        //  判断mark的值进行显示操作
        if(mark)
        {
            int * ptr = p;
            printf("[%d]:%d is a primer.\n",*ptr,i);
        }
        mark = 1;
    }
    pthread_exit(NULL);
}
