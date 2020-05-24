#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage error,need more argument...mycpy <src> <dest>\n");
        exit(1);
    }
    FILE *fps,*fpd;
    int ch,re; //一定要定义int,因为失败的时候返回值是-1,在任何标准C中char型都没有定义符号的类型.

    fps = fopen(argv[1],"r"); //读取文件,还有另外的意思就是源文件必须存在.
    if(fps==NULL)
    {
        perror("fopen(argv[1]):");
        exit(1);
    }
    fpd = fopen(argv[2],"w");
    if(fpd==NULL)
    {
        perror("fopen(argv[2]):");
        exit(2);
    }
    
    while(1)
    {
        ch = fgetc(fps);
        if(ch == EOF)
        {
            puts("OK,FINISHED!\n");
            break;
        }
        re = fputc(ch,fpd);
        if(re == EOF)
        {
            perror("fputc():");
            exit(3);
        }
    }
   
    fclose(fpd);
    fclose(fps); //先关闭依赖别人的文件,再关闭被一来的对象

    exit(0);
}