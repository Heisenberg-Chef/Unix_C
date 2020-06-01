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
    while((ch = getopt(argc,argv,"d:D:?hH"))!= -1){
        switch (ch)
        {
        case 'd':
        case 'D':
            i = atoi(optarg);
            break;
        case '?':
        case 'h':
        case 'H':
            printf("Usage:[-d|-D] [offset day(s)] \n\n");
            break;
        default:
            printf("Usage:[-d|-D] [offset day(s)] \nThis tiny program designed my Heisenberg aka Ray....\n\n");
            break;
        }
    }
    printf("\n\n");
    clac_day(i);
    printf("\n\n");
    exit(0);
}