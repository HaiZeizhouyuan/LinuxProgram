/*************************************************************************
	> File Name: 1.thread_more.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 14时36分35秒
 ************************************************************************/

#include <pthread.h>
#include <stdio.h>

void *func(void *arg) {
    int num = *(int *)arg;
    printf("I'm %d child!\n", num);
}

int main(int argc, char **argv) {
    pthread_t tid[10];
    int tmp_arg[10] = {0};
    for (int i = 0; i < 10; i++) {
        tmp_arg[i] = i;
        pthread_create(&tid[i], NULL, func, (void *)&tmp_arg[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
