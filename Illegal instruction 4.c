#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    char ptr[10] = "test";      //  right
    //  char * ptr = "test";    //  incorrect
    strcpy(ptr,"test");
    return 0;
}