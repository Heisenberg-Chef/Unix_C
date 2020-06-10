#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>



int main()
{
    int fd;
    fd = open("/dev/null",O_RDWR);
    dup2(fd,STDOUT_FILENO);
    printf("File descriptor : %d\n",fd);
    fflush(NULL);
    write(STDOUT_FILENO,"Hello world!\n",13);
    // dup2(1,fd);
    // write(1,"Hello world!\n",13);
    return 0;
}