#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX   1024
typedef void mytbf_t;   // 对于用户的欺骗,其实都是void*而void*可以被给予任何的指针


mytbf_t * mytbf_init(int cps,int brust);

int mytbf_fetchtoken(mytbf_t *, int);

int mytbf_returntoken(mytbf_t *,int);

int mytbf_destory(mytbf_t * );

#endif