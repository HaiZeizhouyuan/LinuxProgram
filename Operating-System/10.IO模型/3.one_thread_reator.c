/*************************************************************************
	> File Name: 3.one_thread_reator.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月12日 星期三 16时10分21秒
 ************************************************************************/

#include "head.h"
#define MAX 10
#define MAXUSER 1024

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage %s port!\n", argv[0]);
        exit(1);
    }

    int server_listen, sockfd, port, epollfd;
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

    struct epoll_event ev, events[MAX];
    ev.data.fd = server_listen;
    ev.events = EPOLLIN;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epol_ctl()");
        exit(1);
    }

    while (1) {
        int nfds = epoll_wait(epollfd, events, MAX, -1);
        if (nfds < 0) {
            perror("epoll_wait()");
            exit(1);
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_listen && ( events[i].events & EPOLLIN)) {
                if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
                    perror("accept()");
                    exit(1);
                }
                fd[sockfd] = sockfd;
                //setnonblocking(fd[sockfd]);
                ev.events = EPOLLIN | EPOLLRDHUP;
                ev.data.fd = sockfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
                    perror("epol_ctl()");
                    exit(1);
                }
            } else {
                if (events[i].events & EPOLLIN) {
                    if (events[i].events & EPOLLRDHUP){
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        DBG("events[i].data.fd = %d\n", events[i].data.fd);
                        close (events[i].data.fd);
                        printf("Logout!\n");
                        break;
                    }
                    char buff[512] = {0};
                    recv(events[i].data.fd, buff, sizeof(buff), 0);
                    printf("recv : %s\n", buff);
                }
                sleep(1);
            }
        }
    }
    

	return 0;
}
