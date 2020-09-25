/*************************************************************************
:x
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
    for (int i = 0; i < THREADNUM; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)&taskQueue);
    }
    while (1) {
        //char buff[BUFFSIZE] = {0};
        char buff;
        scanf("%c",&buff);
        getchar();
        DBG(GREEN"<Dbug>"NONE" : str :%c\n", buff);
        task_queue_push(&taskQueue, buff);
    }
    return 0;
}
