#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main()
{
    int count = 0;
    FILE *fp = NULL;
    while(1)
    {
        fp = fopen("./stdio.c","r");

        if(fp == NULL)
        {
            fprintf(stderr,"fopen() failed! errno = %d\n",errno);
            perror("fopen()");
            fprintf(stderr,"fopen() failed! %s\n",strerror(errno));
            break;
        }
        count++;
    }
    fprintf(stdout,"Max %d files opened,in a process...\n",count);
    //puts("OK!");
    exit(0);
}