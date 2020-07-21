#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static int fd1;



int main(int argc,char ** argv)
{
    fd1 = dup(1);

    printf("fd1 after dup: %d\n",fd1);

    write(1,"abc\n",3);
    write(fd1,"abc",3);
    
    exit(0);
}