#include <stdio.h>
#include <stdlib.h>

int main()
{
    char s[10],* d;
    d = fgets(s,10,stdin);

    printf("%s",d);
}