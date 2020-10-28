/*************************************************************************
	> File Name: client_exit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 17时01分34秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
void client_exit(int signum) {
    DBG(RED"ctrl C!\n"NONE);
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    if (recv(sockfd, (void *)&msg, sizeof(msg), 0) > 0) {
        printf(GREEN "Bye 1\n"NONE);
        exit(0);    
    }
}
