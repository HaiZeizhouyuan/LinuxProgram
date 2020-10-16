/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "thread_pool.h"

void task_queue_init(Task_Queue *taskQueue, int size) {
    taskQueue->data = (char *)malloc(size * sizeof(char));
    taskQueue->head = taskQueue->tail = taskQueue->total = 0;
    taskQueue->size = size;
    //对互斥锁的初始化
    pthread_mutex_init(&taskQueue->mutex, NULL);
    //对变量的初始化
    pthread_cond_init(&taskQueue->cond, NULL);
    return ;
}

void task_queue_push(Task_Queue *taskQueue, char str) {
    pthread_mutex_lock(&taskQueue->mutex);
   // if (taskQueue->total == taskQueue->size) expend(taskQueue);
    taskQueue->data[(taskQueue->tail)++] = str;
    if (taskQueue->tail == taskQueue->size) taskQueue->tail -= taskQueue->size;
    pthread_cond_signal(&taskQueue->cond);
    taskQueue->total += 1;
    pthread_mutex_unlock(&taskQueue->mutex);
    DBG(GREEN"<Dbug>"NONE" : tail : %c\n", taskQueue->data[taskQueue->tail - 1]);
    return ;
}

int empty(Task_Queue *taskQueue) {
    return taskQueue->total == 0;
}

char front(Task_Queue *taskQueue) {
    return taskQueue->data[taskQueue->head];
}

char task_queue_pop(Task_Queue *taskQueue) {
    DBG(GREEN"<Dbug>"NONE" : f = %c, head = %d, tail = %d\n", taskQueue->data[taskQueue->head], taskQueue->head, taskQueue->tail);
    pthread_mutex_lock(&taskQueue->mutex);
    while (empty(taskQueue)) {
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    char f = front(taskQueue);
    if (++taskQueue->head == taskQueue->size) taskQueue->head -= taskQueue->size;
    taskQueue->total -= 1;
    pthread_mutex_unlock(&taskQueue->mutex);
    return f;
}

void *thread_run (void *arg) {
    pthread_detach(pthread_self());
    Task_Queue *taskQueue = (Task_Queue *)arg;
    while (1) {
        DBG(GREEN"<Dbug>"NONE" :Queue have %d task!\n", taskQueue->total);
        char f = task_queue_pop(taskQueue);
        DBG(GREEN"<Dbug>"NONE" : have pop : %c\n", f);
        printf("<%ld> %c\n", pthread_self(), f);
    }
}

