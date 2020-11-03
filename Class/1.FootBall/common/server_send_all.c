/*************************************************************************
	> File Name: server_send_all.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月03日 星期二 16时29分29秒
 ************************************************************************/


#include "head.h"
#define MAX_TEAM 11

struct sockaddr_in client;
socklen_t len = sizeof(client);
extern struct User *rteam, *bteam;
extern WINDOW *Football, *Football_t, *Message, *Help, *Score, *Write;
void send_team(struct User *team, struct FootBallMsg *chat_msg) {
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online) {
            send(team[i].fd, (void *)chat_msg, sizeof(struct FootBallMsg), 0);
        }
    }
}

void send_all(struct FootBallMsg *msg) {
        send_team(rteam, msg);
        send_team(bteam, msg);

}


