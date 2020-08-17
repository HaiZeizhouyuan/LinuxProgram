/*************************************************************************
	> File Name: work_list.h
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:48:13 2020
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "head.h"
#define CHAT_SYN 0x01
#define CHAT_FIN 0x02
#define CHAT_ACK 0x04
#define CHAT_HEART 0x08
#define CHAT_PRI 0x10
#define CHAT_PUB 0x20
#define CHAT_FUNC 0x40
#define CHAT_SYS 0x80

#define FUNC_CHECK_ONLINE 0x01
#define FUNC_CHANGE_NAME 0x02

typedef struct User {
    char name[20];
    char chat_name[20];
    char real_name[20];
    int flag;
    char ip[20];
    int fd;
    int online;
}User;

User users[100];

extern User user;

typedef struct ChatMsg {
    int type;
    int opt;
    char name[20];
    char msg[1024];
} ChatMsg;

typedef struct task_queue {
    int *data;
    int head, tail, total, size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Task_Queue;

void task_queue_init(Task_Queue *taskQueue, int size);
void task_queue_expend(Task_Queue *taskQueue);
void task_queue_push(Task_Queue *taskQueue, int data);
int empty(Task_Queue *taskQueue);
int front(Task_Queue *taskQueue);
int task_queue_pup(Task_Queue *taskQueue);
void clear(Task_Queue *taskQueue);
void *thread_run(void *arg);
#endif
