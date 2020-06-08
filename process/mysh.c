#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

#define DELIMS  " \t\n"

struct cmd_st{
    glob_t globres;
};


void prompt(void)
{
    printf("mysh - 0.1$ ");
}

static void parse(char * line,struct cmd_st * res)
{
    char * tok;
    int i = 0;

    while(1)
    {
        tok = strsep(&line,DELIMS);
        if(tok == NULL)
        {
            break;
        }
        if(tok[0] == '\0')  //多个分隔符连续的可能会解析出来空串
        {
            continue;
        }
        // 定义一个数组来存储.但是使用glob,的GLOB_NOCHECK,直接返回给我不检查通配符
        glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
        i = 1;  //  第一次没有append 以后都有
    }
    
}

int main()
{
    int pid;
    char * linebuf = NULL;
    size_t linebuf_size = 0;
    struct cmd_st cmd;


    while(1)
    {
        prompt();  // 打印提示符

        if(getline(&linebuf,&linebuf_size,stdin) < 0)
        {
            break;
        }

        parse(linebuf,&cmd);

        if(0)    //  内部命令
        {

        }
        else    //  外部命令
        {
            pid = fork();
            if(pid < 0)
            {

            }
            if(pid == 0)
            {
                execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv); // file
                perror("fork()");
                exit(1);
            }
            else{
                wait(NULL);
            }
        }
    }

// cd不能用 属于内部解析的命令

    exit(0);
}