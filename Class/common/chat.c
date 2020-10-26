/*************************************************************************
	> File Name: chat.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月19日 星期三 14时47分25秒
 ************************************************************************/

#include "head.h"

extern struct User *users;
extern int epollfd;
extern int maxfd;
extern int cnt_online;

char str_name[50];
int user_sum;

void add_event(int epollfd, int fd, int events, struct User *user){
    DBG(RED"START add_event!\n"NONE);
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = (void *)user;
    DBG(PINK"Main Reactor"NONE" : adding %s to main reactor\n", user->name);
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        perror("epoll_ctl()");
        exit(1);
    }
	return ; 
}

void del_event(int epollfd, struct User *user) {
    DBG(RED"START dell_event!\n"NONE);
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user->fd, NULL);
    DBG(PINK"Main Reactor"NONE" : deleted %s from main Reactor\n", user->name);
}

int find_sub(struct User *users, int max) {
    DBG(RED"START find_sub!\n"NONE);
    for (int i = 1; i < max; i++) {
        if (!users[i].online) {
            return i;
        }
    }
    return -1;
}

int do_login(struct User *user, struct ChatMsg *msg) {
    DBG(RED"START do_login!\n"NONE);
    int online = 0;
    for (int i = 1; i <= maxfd; i++) {
        if (users[i].online && !strcmp(msg->name, users[i].name)) {
            online = 1;
            break;
        }
    }
    if (online) {
        DBG(YELLOW"Login Error"NONE" : %s is already online\n", msg->name);
        sprintf(msg->msg, "%s is already online!", msg->name);
        msg->type = CHAT_NAK;
        user->online = 0;
        send(user->fd, msg, sizeof(struct ChatMsg), 0);
        close(user->fd);
        del_event(epollfd, user);
        return 1;
    } else {
        DBG(L_GREEN"login Success"NONE " : %s login Success!\n", msg->name);
        sprintf(msg->msg, "%s, you have login Success!", msg->name);
        msg->type = CHAT_ACK;
        send(user->fd, msg, sizeof(msg), 0);
        user->online = 1;
        user->flag = 10; 
        strcpy(user->name, msg->name);
        strcpy(user->chat_name, msg->name);
        cnt_online++;
    } 
    return 0;
    
}

void *heart_beat(void *arg) {
    DBG(RED"START heart_beat!\n"NONE);
    struct ChatMsg msg;
    msg.type = CHAT_HEART;
    while (1) {
        sleep(10);
        for (int i = 1; i <= maxfd; i++) {
            if(users[i].online == 1) {
                send(users[i].fd, (void *)&msg, sizeof(msg), 0);
                if (--users[i].flag <= 0) {
                    DBG(PINK"Heart Beart"NONE" : %s is offline by heart_beat\n", users[i].name);
                    del_event(epollfd, &users[i]);
                    close(users[i].fd);
                    users[i].online = 0;
                    cnt_online--;
                }
            }
        }
    } 
}


