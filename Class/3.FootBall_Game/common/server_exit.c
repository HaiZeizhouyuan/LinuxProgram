/*************************************************************************
	> File Name: server_exit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 11时40分43秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11

extern int server_listen;
extern struct User *red_team, *blue_team;

void server_exit(int signum) {
    DBG(BLUE"LOGOUT"NONE" : Server logout!\n");
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    for (int i = 0; i < MAX_TEAM; i++) {
        if (red_team[i].online) send(red_team[i].fd, (void *)&msg, sizeof(msg), 0);
        if (blue_team[i].online) send(blue_team[i].fd, (void *)&msg, sizeof(msg), 0);
    }
    endwin();
    exit(0);
}
