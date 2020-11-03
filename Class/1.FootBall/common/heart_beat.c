/*************************************************************************
	> File Name: heart_beat.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月09日 星期二 20时21分44秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11

extern struct User *rteam, *bteam;
extern int repollfd, bepollfd;

void *heart_beat(void *arg) {
    while (1) {
        sleep(5);
        heart_beat_team(bteam);
        heart_beat_team(rteam);
    }
}

void heart_beat_team(struct User *team) {
    struct FootBallMsg msg;
    msg.type = FT_HEART;
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online) {
            send(team[i].fd, (void *)&msg, sizeof(msg), 0);
            team[i].flag--;
            if (team[i].flag <= 0) {
                char tmp[512] = {0};
                sprintf(tmp, "%s is removed from list.",team[i].name);
                show_message(NULL, NULL, tmp, 1);
                team[i].online = 0;
                int epollfd_tmp = (team[i].team ? bepollfd : repollfd);
                del_event(epollfd_tmp, team[i].fd);
                close(team[i].fd);

            }
        }
    }
}

