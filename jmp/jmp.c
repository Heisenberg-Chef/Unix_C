
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static void a(void);
static void b(void);
static void c(void);
static void d(void);

static jmp_buf save;

static void a(void)
{
    int ret;
    printf("%s:Begin.\n",__FUNCTION__);
    ret = setjmp(save);
    if(ret = 0)
    {
        printf("%s:Call b().\n",__FUNCTION__);
        b();
        printf("%s:b() returned.\n",__FUNCTION__);
    }else{
        printf("%s:Jumped back here with code %d\n",\
        __FUNCTION__,ret);
    }
    printf("%s:End.\n",__FUNCTION__);

}
static void b(void)
{

    printf("%s:Begin.\n",__FUNCTION__);
    printf("%s:Call c().\n",__FUNCTION__);
    c();
    printf("%s:c() returned.\n",__FUNCTION__);
    printf("%s:End.\n",__FUNCTION__);

}
static void c(void)
{

    printf("%s:Begin.\n",__FUNCTION__);
    printf("%s:Call d().\n",__FUNCTION__);
    d();
    printf("%s:d() returned.\n",__FUNCTION__);
    printf("%s:End.\n",__FUNCTION__);

}

static void d(void)
{

    printf("%s:Begin.\n",__FUNCTION__);
//  longjmp
    printf("%s():Jump now!\n",__FUNCTION__);
    longjmp(save,6);

    // printf("%s:Call a().\n",__FUNCTION__);
    // b();
    // printf("%s:a() returned.\n",__FUNCTION__);
    printf("%s:End.\n",__FUNCTION__);

}

int main()
{

    printf("%s:Begin.\n",__FUNCTION__);
    printf("%s:Call a().\n",__FUNCTION__);
    a();
    printf("%s:a() returned.\n",__FUNCTION__);
    printf("%s:End.\n",__FUNCTION__);

    exit(0);
}