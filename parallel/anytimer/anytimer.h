#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#define JOB_MAX 1024

typedef void at_jobfunc_t(void*);

int at_addjob(int sec,at_jobfunc_t * jobp,void * arg);
/*
return > = 0 成功返回指定任务的ID
        == -EINVAL 失败
        == -ENOSPC 数组满
        == -ENOMEM 内存不足
*/

int at_canceljob(int id);
/* 
return == 0 成功
        == -EINVAL 参数非法


*/



#endif