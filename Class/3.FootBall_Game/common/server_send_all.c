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

void send_team(struct User *team, struct FootBallMsg *msg) {
    DBG(YELLOW"Start Send Wall Mag!\n"NONE);
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online) {
            DBG(BLUE"%s online!\n"NONE, team[i].name);
            send(team[i].fd, (void *)msg, sizeof(struct FootBallMsg), 0);
            // sendto(team[i].fd, (void *)msg, sizeof(msg), 0, (struct sockaddr *)&client, len);
            DBG(BLUE"send msg : %s success!\n"NONE, msg->msg);
        }
    }
}

void send_all(struct FootBallMsg *msg) {
    send_team(red_team, msg);
    send_team(blue_team, msg);
}



