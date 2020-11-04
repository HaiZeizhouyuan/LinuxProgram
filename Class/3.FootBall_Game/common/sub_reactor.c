/*************************************************************************
	> File Name: sub_reactor.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月23日 星期五 10时36分52秒
 ************************************************************************/
#include "head.h"
#define NTHREAD 11
#define MAX_TEAM 11 
void *sub_reactor(void *arg) {
    struct task_queue *taskQueue = (struct task_queue *)arg;
    pthread_t *tid = (pthread_t *)calloc(NTHREAD, sizeof(pthread_t));
    for (int i = 0; i < NTHREAD; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)taskQueue);
    }
    struct epoll_event ev, events[MAX_TEAM];
    sigset_t sigmask, origmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGALRM);
    while(1) {
        DBG(RED"Sub Rector"NONE": Epoll Waiting...\n");
        pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
        int nfds = epoll_wait(taskQueue->epollfd, events, MAX_TEAM, -1);
        pthread_sigmask(SIG_SETMASK, &origmask, NULL);
        if (nfds == -1) {
            perror("epoll wait sub_reactor");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].events & EPOLLIN) {
                task_queue_push(taskQueue, (struct User *)events[i].data.ptr);
            }
        }
    }
    return NULL;
}
