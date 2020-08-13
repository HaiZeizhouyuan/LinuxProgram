/*************************************************************************
	> File Name: epoll.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月13日 星期四 09时21分06秒
 ************************************************************************/
//include "head.h"
#define MAX_EVENTS 10

struct epoll_event ev, events[MAX_EVENTS];
int listen_sock, conn_sock, nfds, epollfd;

epollfd = epoll_create(0);
if (epollfd== -1) {
    perror("epoll_create()");
    exit(1);
}

ev.events = EPOLLIN;
ev.data.fd = listen_sock;

if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
    perror("epoll_ctl : listen_sock");
    exit(1);
}

while (1) {
    ndfs = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (ndfs == -1) {
        perror("epoll_wait");
        exit(1);
    }

    for (int i = 0; i < nfds; ++n) {
        if (events[n].data.fd == listen_sock) {
            conn_sock = accept(listen_sock, (struct sockaddr *)&addr, &addrlen);
            if (conn_sock == -1) {
                perror("accept()");
                exit(1);
            }
            setnonblocking(coon_sock);
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_sock;
            if (epoll_ctl(conn_sock, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                perror("epoll_ctl :conn_sock");
                exit(1);
            } else {
                do_use_fd(events[n].data.fd);
            }
        }
    }
}


