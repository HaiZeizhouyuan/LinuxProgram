/*************************************************************************
	> File Name: server_exit.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月13日 星期六 14时44分44秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
extern struct User *rteam, *bteam;

void server_exit(int signum) {
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    for (int i = 0; i < MAX_TEAM; i++) {
        if (rteam[i].online) send(rteam[i].fd, (void *)&msg, sizeof(msg), 0);
        if (bteam[i].online) send(bteam[i].fd, (void *)&msg, sizeof(msg), 0);
    }
    DBG(RED"Server stopped!\n"NONE);
    endwin();
    exit(0);
}
