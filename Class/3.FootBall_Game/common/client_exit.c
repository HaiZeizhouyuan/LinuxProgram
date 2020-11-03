/*************************************************************************
	> File Name: client_exit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 17时01分34秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
struct FootBallMsg chat_msg;
void client_exit(int signum) {
    DBG(RED"ctrl C!\n"NONE);
    struct User user;
    chat_msg.type = FT_FIN;
    user.team = chat_msg.team;
    strcpy(user.name, chat_msg.name);
    send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
    close(sockfd);
    endwin();
    exit(0);    
}
