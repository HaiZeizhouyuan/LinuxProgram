/*************************************************************************
	> File Name: chat_server.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月12日 星期三 16时10分21秒
 ************************************************************************/

#include "work_list.h"
#define MAXEVENTS 10
#define MAXUSER 1024
#define QUEUESIZE 50
#define THREAD_NUM 10

int epollfd;
User user;
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage %s port!\n", argv[0]);
        exit(1);
    }

    int server_listen, sockfd, port;
    int fd[MAXUSER] = {0};
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {//创建服务端
        perror("socket_create()");
        exit(1);
    }

    if ((epollfd = epoll_create(1)) < 0) {
        perror("epoll_create()");
        exit(1);
    }

    struct epoll_event ev, events[MAXEVENTS];
    
    ev.data.fd = server_listen;
    ev.events = EPOLLIN | EPOLLET;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epol_ctl()");
        exit(1);
    }

    pthread_t *tid = (pthread_t *)calloc(THREAD_NUM, sizeof(pthread_t));
    Task_Queue taskQueue;
    task_queue_init(&taskQueue, QUEUESIZE);
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&tid[i], NULL, thread_run, (void *)&taskQueue);
    }

    while (1) {
        usleep(100);
        int nfds = epoll_wait(epollfd, events, MAXEVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait()");
            exit(1);
        }
        DBG(YELLOW"<Dbug>"NONE" : After wait nfds = %d\n", nfds);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_listen && ( events[i].events & EPOLLIN)) {
                if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
                    perror("accept()");
                    exit(1);
                }
                fd[sockfd] = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = sockfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
                    perror("epol_ctl()");
                    exit(1);
                }
            } else {
                if (events[i].events & EPOLLIN) {                
                    task_queue_push(&taskQueue, events[i].data.fd);
                }
            }
        }
    }
	return 0;
}
