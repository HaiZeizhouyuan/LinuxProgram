/*************************************************************************
	> File Name: heart_beat.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 15时27分02秒
 ************************************************************************/

#ifndef _HEART_BEAT_H
#define _HEART_BEAT_H
#define MAX_TEAM 11
extern struct User *red_team, *blue_team;
extern int red_epollfd, blue_epollfd;
void heart_beat_team(struct User *team);
void *heart_beat(void *arg);
#endif
