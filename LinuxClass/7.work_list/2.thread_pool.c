/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "thread_pool.h"

void task_queue_init(Task_Queue *taskQueue, int size) {
    taskQueue->data = (char **)malloc(size * sizeof(char*));
    taskQueue->head = taskQueue->tail = taskQueue->total = 0;
    taskQueue->size = size;
    //对互斥锁的初始化
    pthread_mutex_init(&taskQueue->mutex, NULL);
    //对变量的初始化
    pthread_cond_init(&taskQueue->cond, NULL);
    return ;
}

void expend(Task_Queue *taskQueue) {
    DBG(GREEN"<Dbug>"NONE ": start expend!\n");
    int extr_size = taskQueue->size;
    char **p;
    while (extr_size) {
        p = (char **)malloc(sizeof(char *) * (taskQueue->size + extr_size));
        if (p) break;
        extr_size >>= 1;
    }
    for (int i = taskQueue->head, j = 0; j < taskQueue->total; j++ ) {
        p[j] = taskQueue->data[(i + j) % taskQueue->size];
    }

    free(taskQueue->data);
    taskQueue->data = p;
    taskQueue->head = 0;
    taskQueue->tail = taskQueue->total;
    taskQueue->size += extr_size;
    DBG(GREEN"<Dbug>"NONE" : expend success!\n");
    return ;

}

void task_queue_push(Task_Queue *taskQueue, char *str) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) expend(taskQueue);
    taskQueue->data[(taskQueue->tail)++] = str;
    if (taskQueue->tail == taskQueue->size) taskQueue->tail -= taskQueue->size;
    taskQueue->total += 1;
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
    return ;
}

int empty(Task_Queue *taskQueue) {
    return taskQueue->total == 0;
}

char *front (Task_Queue *taskQueue) {
    return taskQueue->data[taskQueue->head];
}

char *task_queue_pop(Task_Queue *taskQueue) {
    DBG(GREEN"<Dbug>"NONE" : f = %s, head = %d, tail = %d\n", taskQueue->data[taskQueue->head], taskQueue->head, taskQueue->tail);
    pthread_mutex_lock(&taskQueue->mutex);
    while (empty(taskQueue)) {
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    char *f = taskQueue->data[taskQueue->head];
    if (++taskQueue->head == taskQueue->size) taskQueue->head -= taskQueue->size;
    taskQueue->total -= 1;
    pthread_mutex_unlock(&taskQueue->mutex);
    return f;
}

void clear(Task_Queue *taskQueue) {
    if (taskQueue == NULL) return ;
    free(taskQueue->data);
    free(taskQueue);
    return ;
}

void *thread_run (void *arg) {
    pthread_detach(pthread_self());
    Task_Queue *taskQueue = (Task_Queue *)arg;
    while (1) {
        DBG(GREEN"<Dbug>"NONE" :Queue have %d task!\n", taskQueue->total);
        char *str = task_queue_pop(taskQueue);
        DBG(GREEN"<Dbug>"NONE" : have pop : %s\n", str);
        printf("<%ld> %s\n", pthread_self(), str);
    }
}

