/*************************************************************************
	> File Name: 3.one_thread_reator_1.c
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
                ev.events = EPOLLIN;
                ev.data.fd = sockfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
                    perror("epol_ctl()");
                    exit(1);
                }
            } else {
                if (events[i].events & EPOLLIN) {
                    char buff[512] = {0};
                    ssize_t rev;
                    rev = recv(events[i].data.fd, buff,sizeof(buff), 0);

                    if (rev < 0) {
                        perror("recv()");
                        exit(1);
                    }
                    if (rev == 0) {
                       ev.events = EPOLLIN;
                        ev.data.fd = events[i].data.fd;
                        epoll_ctl(events[i].data.fd, EPOLL_CTL_DEL, sockfd, &ev);
                        close(events[i].data.fd);
                        printf("Bye Bye\n");
                        break;
                    }
                    printf("recv : %s\n", buff);
                }
            }
        }
    }
	return 0;
}
