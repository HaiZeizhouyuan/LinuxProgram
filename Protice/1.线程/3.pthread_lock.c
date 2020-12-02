/*************************************************************************
	> File Name: 3.pthread_lock.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 15时10分33秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <pthread.h>

#define MAX 1000
int now = 0, sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *add(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (now > MAX) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        sum += now;
        now += 1;
        pthread_mutex_unlock(&mutex);
        printf("<%ld> now :%d, sum : %d\n", pthread_self(), now, sum);
        usleep(10);
    }
}

int main() {
    pthread_t tid[10];
    for (int i = 0; i < 10;i++) {
        pthread_create(&tid[i], NULL, add, NULL);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
