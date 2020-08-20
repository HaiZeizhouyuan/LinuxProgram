/*************************************************************************
	> File Name: 3.test.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 09:03:47 2020
 ************************************************************************/

#include "thread_pool.h"

#define SIZE 50
#define THREADNUM 5
#define BUFFSIZE 512

int main () {
    pthread_t tid[THREADNUM];
    Task_Queue taskQueue;
    task_queue_init(&taskQueue, SIZE);
    char buff[SIZE][BUFFSIZE] = {0};
    for (int i = 0; i < THREADNUM; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)&taskQueue);
    }
    int sub = 0;
    while (1) {
        FILE *fp = fopen("./2.thread_pool.c", "r");
        if (fp == NULL) {
            perror("fopen()");
            exit(1);
        }
        while (fgets(buff[sub], BUFFSIZE, fp) != NULL) {
            usleep(100);
            DBG(BLUE"<str> "NONE" : %s\n", buff[sub]);
            task_queue_push(&taskQueue, buff[sub]);
            if (++sub >= SIZE) sub = 0;
        }
        fclose(fp);
    }
    return 0;
}
