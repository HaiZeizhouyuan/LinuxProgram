/*************************************************************************
	> File Name: thread_pool.h
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:48:13 2020
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "head.h"
typedef struct task_queue {
    char **data;
    int head, tail, total, size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Task_Queue;

void task_queue_init(Task_Queue *taskQueue, int size);
void task_queue_expend(Task_Queue *taskQueue);
void task_queue_push(Task_Queue *taskQueue, char *str);
int empty(Task_Queue *taskQueue);
char *front(Task_Queue *taskQueue);
char *task_queue_pup(Task_Queue *taskQueue);
void clear(Task_Queue *taskQueue);
void *thread_run(void *arg);
#endif
