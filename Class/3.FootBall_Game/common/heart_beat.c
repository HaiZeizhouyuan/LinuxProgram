/*************************************************************************
	> File Name: heart_beat.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 15时27分34秒
 ************************************************************************/

#include "head.h"

void *heart_beat(void *arg) {
    while(1) {
        sleep(5);
        heart_beat_team(red_team);
        heart_beat_team(blue_team);
    }
}

void heart_beat_team(struct User *team) {
    struct FootBallMsg msg;
    msg.type = FT_HEART;
    struct sockaddr_in client;
    socklen_t len = sizeof(client); 
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online) {
            if (team[i].flag <= 0) {
                DBG(RED"Heart Beart"NONE" : %s is offline by heart_beat\n", team[i].name);
                del_event(red_epollfd, team[i].fd);
                close(team[i].fd);
                team[i].online = 0;
               // cnt_online--;
            }
            sendto(team[i].fd, (void *)&msg, sizeof(msg), 0, (struct sockaddr *)&client, len);
            team[i].flag -= 1;
        }
    }
}

