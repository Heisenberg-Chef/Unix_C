#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char ** argv)
{
    pid_t pid;
    if(argc<3)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    pid = fork();

    if(pid < 0)
    {

    }

    if(pid == 0)
    {
        //child
        setuid(atoi(argv[1]));  // 设置自己的有效id
        execvp(argv[2],argv+2); // char ** 
        perror("execvp()");
        exit(1);
    }

    wait(NULL);

    exit(0);
}

//chmod chown u+s