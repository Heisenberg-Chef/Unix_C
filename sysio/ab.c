#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    putchar('a');
    fflush(NULL);
    write(1,"b",1);

    putchar('a');
    write(1,"b",1);

    putchar('a');
    write(1,"b",1);

    exit(0);
}