/*************************************************************************
	> File Name: server_exit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 11时40分43秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11

extern struct User *red_team, *blue_team;

void server_exit(int signum) {
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    for (int i = 0; i < MAX_TEAM; i++) {
        if (red_team[i].online) send(red_team[i].fd, (void *)&msg, sizeof(msg), 0);
        if (blue_team[i].online) send(blue_team[i].fd, (void *)&msg, sizeof(msg), 0);
    }
    endwin();
    refresh();
    exit(0);
}
