/*************************************************************************
	> File Name: server_send_all.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 14时13分44秒
 ************************************************************************/

#ifndef _SERVER_SEND_ALL_H
#define _SERVER_SEND_ALL_H

void send_team(struct User *team, struct FootBallMsg *msg);
void send_all(struct FootBallMsg *msg);
#endif
