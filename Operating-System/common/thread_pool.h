/*************************************************************************
    > File Name: thread_pool.h
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月21日 星期五 08时43分19秒
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H


struct task_queue {
    int head, tail, total, size, epollfd;
    struct User **users;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int size, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
int empty(struct task_queue *taskQueue);
struct User *front(struct task_queue *taskQueue);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);
void do_work(struct User *user);

#endif
