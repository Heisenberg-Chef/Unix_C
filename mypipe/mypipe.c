#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mypipe.h"

struct mypipe_st
{
    int head;
    int tail;
    char data[PIPESIZE];
    //  队列可以没有尾 0
    int datasize;
    pthread_mutex_t mut;
    pthread_cond_t cond; // 非忙等
    int count_rd;//读者计数,计数器的意义就在于记录还有没有人需要
    int count_wr;//写者计算
};

static int next(struct mypipe_st * me)
{
    if(me->head >= me->tail)
        return -1;
    else
    {
        me->head++;
    }

    return me->head;
}

int mypipe_register(mypipe_t * ptr,int opmap)
{
    //if error
    struct mypipe_st * me = ptr;
    pthread_mutex_lock(&me->mut);
    if(opmap & MYPIPE_READ)
    {
        me->count_rd++;
    }
    if(opmap & MYPIPE_WRITE)
    {
        me->count_wr++;
    }

    pthread_cond_broadcast(&me->cond);
    while(me->count_rd <=0 || me->count_wr <= 0)
    {
        pthread_cond_wait(&me->cond,&me->mut);
    }

    pthread_mutex_unlock(&me->mut);
    return 0;
}

int mypipe_unregister(mypipe_t * ptr,int opmap)
{
        struct mypipe_st * me = ptr;
    pthread_mutex_lock(&me->mut);
    if(opmap & MYPIPE_READ)
    {
        me->count_rd--;
    }
    if(opmap & MYPIPE_WRITE)
    {
        me->count_wr--;
    }

    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return 0;
}

static int mypipe_readbyte_unlock(struct mypipe_st * me ,char *datap)
{
    //不上锁的版本[man 中的标准相似]
    if(me->datasize<=0)
        return -1;
    *datap = me -> data[me->head];
    me->head = next(me->head);
    me->datasize--;
    return 0;
}

mypipe_t * mypipe_init(void)
{
    struct mypipe_st * me;

    me = malloc(sizeof(*me));
    if(me == NULL)
        return NULL;

    me->head = 0;
    me->tail = 0;
    me->datasize = 0;
    me->count_wr = 0;
    me->count_wr = 0;
    pthread_mutex_init(&me->mut,NULL);
    pthread_cond_init(&me->cond,NULL);

    return me;
}

int mypipe_read(mypipe_t * ptr,void * buf,size_t size)
{
    int i;
    struct mypipe_st * me = ptr;

    pthread_mutex_lock(&me->mut);
    while(me->datasize <= 0 || me->count_wr > 0)    // 并且有写者 才等待 没有写者读者需要退出
    {
        pthread_cond_wait(&me->cond,&me->mut);
    }

    if(me->datasize<=0 && me->count_wr <= 0)
    {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for(i=0;i<size;i++)
    {
        if(mypipe_readbyte_unlock(me,buf+i) != 0)
            break;
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return i;
}

int mypipe_write(mypipe_t * ptr,const void * buf ,size_t size)
{
    
}

int mypipe_destroy(mypipe_t * ptr)
{
    struct mypipe_st * me = ptr;

    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);

    return 0;
}


