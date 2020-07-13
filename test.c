// GDB调试器测试案例,通过这个学习调试程序.
#include <stdio.h>
#include <stdlib.h>

int sum(int value);
 
struct inout {
    int value;
    int result;
};

int main(int argc, char * argv[])
{
    struct inout * io = (struct inout * ) malloc(sizeof(struct inout));
    if (NULL == io) {
        printf("Malloc failed.\n");
        return -1;
    }

    if (argc != 2) {
        printf("Wrong para!\n");
        return -1;
    }

    io -> value = *argv[1] - '0';
    io -> result = sum(io -> value);
    printf("Your enter: %d, result:%d\n", io -> value, io -> result);
    return 0;
}

int sum(int value) {
    int result = 0;
    int i = 0;
    for (i = 0; i < value; i++)
        result += (i + 1);
    return result;
}