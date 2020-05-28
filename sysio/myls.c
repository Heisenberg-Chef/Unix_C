#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static long long flen(const char *fname)    //禁止外部扩展.
{
    struct stat statres;
    if(stat(fname,&statres)<0)
    {
        perror("stat():");
        exit(1);
    }
    return statres.st_size;
}


int main(int argc,char ** argv)
{
    struct stat statres;

    if(argc != 2)
    {
        fprintf(stderr,"Usage ...\n");
        exit(1);
    }
    fprintf(stdout,"file name : %20s\nfile size : %20lld\n",argv[1],flen(argv[1]));
    
    exit(0);
}