#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TIMESTRSIZE 1024

static time_t stamp;
static struct tm * tmp;
static char timestr[TIMESTRSIZE],ch;
static int i;
static void clac_day(int i)
{

    stamp = time(NULL);
    tmp = localtime(&stamp);
    printf("----------------Now time----------------\n");
    strftime(timestr,TIMESTRSIZE,"Now:%Y-%m-%d Month:%B in Weekday:%A",tmp);
    puts(timestr);

    tmp->tm_mday += i;
    mktime(tmp);
    printf("------------After %d day(s)------------\n",i);
    strftime(timestr,TIMESTRSIZE,"Now:%Y-%m-%d Month:%B in Weekday:%A",tmp);
    puts(timestr);
}

int main(int argc,char ** argv)
{
    //  -n -N
    while((ch = getopt(argc,argv,"n:N:"))!= -1){
        switch (ch)
        {
        case 'n':
        case 'N':
            i = atoi(optarg);
            break;
        default:
            break;
        }
    }

    clac_day(i);
    exit(0);
}