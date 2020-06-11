#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static unsigned long long count;

int main()
{
    time_t end;

    end = time(NULL)+5;

    while(time(NULL)<end)
    {
        count++;
    }
    printf("count = %lld\n",count);
}