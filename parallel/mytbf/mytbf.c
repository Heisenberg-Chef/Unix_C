#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
//本地头文件
#include "mytbf.h"

//  静态区变量
static int inited = 0;
static struct mytbf_st * job[MYTBF_MAX];
static sig_t alrm_handler_save;

struct mytbf_st{
    int cps;
    int brust;
    int token;
    int pos;
};

static int min(int a,int b)
{
    return a<b?a:b;
}

static int get_free_pos(void)
{   
    for(int i = 0;i < MYTBF_MAX;i++)
    {
        if(job[i] == NULL)
        {
            return i;
        }
    }
    return -1;//失败返回 小于 0;

}

static void alrm_handler(int s)
{

    alarm(1);

    for(int i = 0;i < MYTBF_MAX;i++)
    {
        if(job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if(job[i]->token > job[i]->brust)
                job[i]->token = job[i]->brust;
        }
    }
}


static void module_unload(void) //释放我的非正常结束的令牌桶,把我们的alarm功能改回来.
{
    signal(SIGALRM,alrm_handler_save);   // 这里不能用SIG_DEL,不能确定别人用了没...
    alarm(0);
    for(int i =0;i < MYTBF_MAX;i++)
    {
        free(job[i]);   //free一个空指针没有影响
    }
}

static void module_load(void)
{
    alrm_handler_save = signal(SIGALRM,alrm_handler);
    alarm(1);

    // 把我们非正常结束挂在钩子上,只有在结束的时候才清空所有的功能.
    atexit(module_unload);
}

mytbf_t * mytbf_init(int cps,int brust)
{

    int pos;
    struct mytbf_st * me;

    if(!inited)
    {
        module_load();
    }
    
    pos = get_free_pos();
    if(pos<0)
        return NULL;
    me = malloc(sizeof(*me));

    if(me == NULL)
        return NULL;

    me->token = 0;
    me->cps = cps;
    me->brust = brust;
    me->pos = pos;

    job[pos] = me;  // 静态区的指针数组

    return me;
}

int mytbf_fetchtoken(mytbf_t * ptr, int size)
{   
    int n;
    struct mytbf_st * me =ptr;
    if(size <= 0)
    {
        return -EINVAL;
    }
    while(me->token <= 0)
        pause();    // 阻塞到有token为止
    
    n = min(me->token,size);
    me->token -= n;
    return me->token;
}

int mytbf_returntoken(mytbf_t * ptr,int size)
{
    struct mytbf_st * me = ptr;
    if(size <= 0)
    {
        return -EINVAL;
    }
    
    me->token += size ;
    if(me->token > me->brust)
        me->token = me->brust;
        //用户还给我几个我就加几个,但是超过brust的话我会告诉他成功
    return size;

}

int mytbf_destory(mytbf_t * ptr)
{
    // void * 的指针是不可以进行取值操作的.因此对他进行一个转换
    struct mytbf_st * me = ptr;
    
    job[me->pos] = NULL;   
    free(ptr);
    return 0;
}