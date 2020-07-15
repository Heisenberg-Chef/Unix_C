#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main()
{
// gcc 默认会忽略 要加上 -fopenmp
#pragma omp parallel sections        //  使用语法标准告诉编译器使用并发
{
#pragma omp section
    printf("[%d]:Hello",omp_get_thread_num());
#pragma omp section
    printf("[%d]:World",omp_get_thread_num());
}
    return 0;
}