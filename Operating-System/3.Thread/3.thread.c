/*************************************************************************
	> File Name: 3.thread.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 18:54:30 2020
 ************************************************************************/

#include "head.h"
#define INS 5
#define max_n 1000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int now = 0, sum = 0;


void *add(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (now >= max_n) {
            pthread_mutex_unlock(&mutex);
            break;
        } 
        now++;
        sum += now;
        pthread_mutex_unlock(&mutex);
        printf("<%ld>%d\n",pthread_self(),  now);
        usleep(20);//微秒
    }
}

int main () {
    pid_t pid;
   
    pthread_t tid[INS];
    int path_i[5];
    for (int i = 0; i < INS; i++) {
            pthread_create(&tid[i], NULL, add, NULL);
    }

    for (int i = 0; i < INS; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("sum = %d\n", sum);
    return 0;
}
