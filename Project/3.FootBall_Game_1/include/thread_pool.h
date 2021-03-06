/*************************************************************************
	> File Name: thread_pool.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月23日 星期五 12时22分30秒
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
struct task_queue {
    int size;
    int total;
    int epollfd; //从反应堆
    struct User **team;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int size, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);
void *draw(void *arg);
void map_change();
#endif
