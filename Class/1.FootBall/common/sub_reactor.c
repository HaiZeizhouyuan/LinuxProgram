/*************************************************************************
	> File Name: common/sub_reactor.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月09日 星期二 19时51分38秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
#define NTHREAD 11

void *sub_reactor(void *arg) {
    struct task_queue *taskQueue = (struct task_queue *)arg;
    pthread_t *tid = (pthread_t *)calloc(NTHREAD, sizeof(pthread_t));
    for (int i = 0; i < NTHREAD; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)taskQueue);
    }
    struct epoll_event ev, events[MAX_TEAM];
    
    while (1) {
        DBG(RED"Sub Reactor: "NONE " Epoll waiting...\n");
        int nfds = epoll_wait(taskQueue->epollfd, events, MAX_TEAM, -1);
        if (nfds == -1) {
            perror("epoll_wait sub_reactor");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            struct User *user = (struct User *)events[i].data.ptr;
            DBG(L_YELLOW"events[n].data.ptr->name = %s \n"NONE, user->name);
            if (events[i].events & EPOLLIN) {
                task_queue_push(taskQueue, (struct User *)events[i].data.ptr);
            }
        }
    }
    return NULL;
}
