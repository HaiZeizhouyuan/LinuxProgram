/*************************************************************************
	> File Name: udp_epoll.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 23时19分37秒
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
#define MAX_TEAM 11

void add_event_ptr(int epollfd, int fd, int events, struct User *user);
void del_event(int epollfd, int fd);
int udp_connect(struct sockaddr_in *client);
int udp_accept(int fd, struct User *user);
void add_to_sub_reactor(struct User *user);
int check_online(struct LogRequest *request);
#endif
