/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 16:28:42 2020
 ************************************************************************/

#include "head.h"
#define SIZE 50
#define THREADNUM 5
#define BUFFSIZE 512

int main () {
    struct task_queue taskQueue;
    task_queue_init(&taskQueue, SIZE);

    for (int i = 0; i < THREADNUM; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)&taskQueue);
    }

    while (1) {
        char buff[BUFFSIZE] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        task_queue_pushj(&taskQueue, buff);
    }


    return 0;
}
