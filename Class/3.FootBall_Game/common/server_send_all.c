/*************************************************************************
	> File Name: server_send_all.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 14时14分31秒
 ************************************************************************/
#include "head.h"
#define MAX_TEAM 11

struct sockaddr_in client;
socklen_t len = sizeof(client);
extern struct User *red_team, *blue_team;
extern WINDOW *Football, *Football_t, *Message, *Help, *Score, *Write;
void send_team(struct User *team, struct FootBallMsg *chat_msg) {
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online) {
            send(team[i].fd, (void *)chat_msg, sizeof(struct FootBallMsg), 0);
        }
    }
}

void send_all(struct FootBallMsg *msg) {
    send_team(red_team, msg);
    send_team(blue_team, msg);
}



