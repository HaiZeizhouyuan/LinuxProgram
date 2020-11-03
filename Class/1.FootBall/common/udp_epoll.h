/*************************************************************************
	> File Name: ../common/udp_epoll.h
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月04日 星期四 19时24分01秒
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
#include "datatype.h"
void add_event_ptr(int epollfd, int fd, int events, struct User *user);
void del_event(int epollfd, int fd);
int udp_connect(struct sockaddr_in *client);
int udp_accept(int fd, struct User *user);
void add_to_sub_reactor(struct User *user);
int check_online(struct LogRequest *request);
#endif
