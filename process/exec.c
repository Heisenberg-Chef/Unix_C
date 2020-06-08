#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    int err;
    //date +s%  --> time_t 那个大时戳
    puts("Begin.\n");
    err = execl("/bin/date","+%s",NULL);
    //第一个是可执行二进制文件的位置,后面的都是传入的参数,以NULL结束.

    //如果调用成功了,后面的都执行不到了.    就是传说中的我已经不再是我了.
    perror("execl()");
    exit(1);
    puts("End.\n");
    exit(0);
}
//如果重定向的话,当前的begin就不见了.最好注意用fflush()的使用.