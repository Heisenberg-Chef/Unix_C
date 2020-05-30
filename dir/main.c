#include <stdio.h>
#include <stdlib.h>


int main(int argc,char ** argv)
{
    printf("argc = %30d\n",argc);
    for(int i = 0;i<argc;i++)
        printf("argv = %30s\n",argv[i]);
    exit(0);
}