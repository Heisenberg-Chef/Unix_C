#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#define PAT "/etc/*"  // 必须写*,不包括隐藏的
#if 0 //如果我想调用失败函数,可以这么调用结果
static int errfunc_(const char * errpath,int eerrno)
{
    puts(errpath);
    fprintf(stderr,"ERROR MSG: %s\n",strerror(errno));
    return 0;
}
#endif
int main()
{
    glob_t globres;

    int err = glob(PAT,0,NULL,&globres);
    if(err)
    {
        printf("Error code = %d\n",err);
        exit(1);
    }

    for(int i=0;i<globres.gl_pathc;i++)
    {
        puts(globres.gl_pathv[i]);
    }
    globfree(&globres);
    exit(0);
}