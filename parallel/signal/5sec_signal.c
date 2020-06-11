#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
/*在C当中使用优化,他只会检测到自己内存中的数据,
如果有信号那么他会优化错误,
循环体中没有用到loop但是没有考虑到信号的影响
要使用关键词volatile 告诉编译器这个关键词的数值要在
内存实际的地址取数值而不是分析代码段.
*/
static volatile int loop = 1;    // true 告诉变意思要从内存取数据

static void alarm_handler(int s)
{
    loop = 0;   // set false
    printf("parameter -> s = %d\nsignal is %d\n",s,SIGALRM);
}

int main()
{
    int64_t count = 0 ;

    signal(SIGALRM,alarm_handler);
    alarm(5);

    while(loop)
    {
        count++;
    }

    printf("count : %lld\n",count);
}
