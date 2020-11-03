/*************************************************************************
	> File Name: thread_pool.h
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月07日 星期日 20时08分31秒
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
struct task_queue{
    int sum;
    int total;
    int epollfd;
    struct User **team;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);

#endif
