/*************************************************************************
	> File Name: client_recver.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月13日 星期六 14时14分49秒
 ************************************************************************/

#include "head.h"

extern int sockfd;

void *client_recv(void *arg) {
    while (1) {
        struct FootBallMsg msg;
        struct User user;
        bzero(&msg, sizeof(msg));
        
        int ret = recv(sockfd, (void *)&msg, sizeof(msg), 0);  
        if (ret <= 0) {
            perror("client_recv");
            exit(1);
        }
        strcpy(user.name, msg.name);
        user.team = msg.team;
        if (msg.type & FT_HEART) {
            DBG(RED"HeartBeat from server 💓\n"NONE);
            msg.type = FT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & FT_MSG) {
            DBG(GREEN"server Msg : "NONE"%s\n", msg.msg);
            show_message(NULL, &user, msg.msg, 0);
        } else if (msg.type & FT_WALL) {
            show_message(NULL, NULL, msg.msg, 1);
        } else if (msg.type &FT_FIN) {
            DBG(GREEN"Server is going tp stop.\n"NONE);
            close(sockfd);
            endwin();
            exit(1);
        } else if (msg.type & FT_MAP){
            show_message(NULL, &user, "FootBallMsg Game refresh", 1);
            //parse_spirit(msg.msg, msg.size);
        } else {
            DBG(GREEN"Msg Unsupport!\n "NONE);
        }
    }
}

