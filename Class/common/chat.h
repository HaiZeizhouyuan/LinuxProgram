/*************************************************************************
	> File Name: chat.h
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月19日 星期三 14时29分45秒
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H
#define MAXUSER 100
#define MAXEVENTS 10
#define THREADNUM 5

void add_event(int epollfd, int fd, int events, struct User *user);
void del_event(int epollfd, struct User *user);
int find_sub(struct User *users, int max);
int do_login(struct User *user, struct ChatMsg *msg);
void *heart_beat(void *arg);
void user_num();
#endif
