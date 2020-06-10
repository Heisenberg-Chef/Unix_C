#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char ** argv)
{
    FILE *fp;
    char *linebuf = NULL;
    size_t linesize = 0;



    fp = fopen("core.c","r");
    
    fp = NULL;

    fread(&linebuf,10,10,fp);

    while(1)
    {
        if(getline(&linebuf,&linesize,fp)<0)
        {
            break;
        }
        printf("%lu \n",strlen(linebuf));
    }
    exit(0);
}