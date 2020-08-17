/*************************************************************************
	> File Name: test.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月16日 星期日 18时25分24秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define true 1

void * One(void * no) 
{
    while (true)
    {
    printf("NUAACS1\n");
    sleep(1);
    }
}

void * Two(void * no) 

{
    while (true)
    {
    printf("NUAACS2\n");
    sleep(1);
    }
}

void Stop(int signo) 
{
    printf("oops! stop!!!\n");
    _exit(0);
}

int main()
{
    int res;
    pthread_t A, B;

    signal(SIGINT, Stop); 
    res = pthread_create(&A, NULL, One, NULL);
    res = pthread_create(&B, NULL, Two, NULL);
    res = pthread_join(A, NULL);
    res = pthread_join(B, NULL);

    return 0;
}
