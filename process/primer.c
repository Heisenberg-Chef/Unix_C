#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 300000000
#define RIGHT 300000200

int main()
{
    int mark;
    for(long i = LEFT;i<=RIGHT;i++)
    {
        int mark = 1;
        for(long j = 2;j<i/2;j++)
        {
            if(i%j == 0)
            {
                mark = 0;
                break;
            }
        }
        if(mark)
        {
            printf("%ld is a primer.\n",i);
        }
    }



    exit(0);
}