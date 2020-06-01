#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIMESTRSIZE 1024

int main()
{
    time_t stamp;
    struct tm * tmp;
    char timestr[TIMESTRSIZE];

    int i = 100;

    stamp = time(NULL);
    tmp = localtime(&stamp);
    strftime(timestr,TIMESTRSIZE,"Now:%Y-%m-%d Month:%B in Weekday:%A",tmp);
    puts(timestr);

    tmp->tm_mday += i;
    mktime(tmp);
    printf("-------After %d day(s)-------\n");
    strftime(timestr,TIMESTRSIZE,"Now:%Y-%m-%d Month:%B in Weekday:%A",tmp);
    puts(timestr);
    exit(0);
}