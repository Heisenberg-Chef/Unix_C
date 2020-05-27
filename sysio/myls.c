#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static int flen(char *fname)    //禁止外部扩展.
{

}


int main(int argc,char ** argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage ...\n");
        exit(1);
    }
    exit(0);
}