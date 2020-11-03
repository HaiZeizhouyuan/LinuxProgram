/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 21时14分49秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
void *client_recv(void *arg) {
    struct FootBallMsg chat_msg;
    struct User user;
    while(1) {
        bzero(&chat_msg, sizeof(chat_msg));
        bzero(&user, sizeof(user));
        int ret = recv(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
        if (ret != sizeof(chat_msg)) {
            perror("client_recv()");
            exit(1);
        }
        user.team = chat_msg.team;
        strcpy(user.name, chat_msg.name);
        DBG(BLUE"Have msg recv!, type = %d\n"NONE, chat_msg.type);
        DBG(RED"wall : %d, msg : %d, fin : %d , heart : %d\n"NONE, FT_WALL, FT_MSG, FT_FIN, FT_HEART);
        if (chat_msg.type & FT_HEART) {//心跳
            DBG(PINK"❤"NONE": Heart Beat From Server!\n");
            chat_msg.type = FT_ACK;
            send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
        } else if (chat_msg.type & FT_FIN) {//服务端下线
            DBG(GREEN"FIN"NONE" : Server Have Logout!\n");
            show_message(NULL, NULL, "Server Have Logout!\n", 1);
            close(sockfd);
            endwin();
            exit(0);
        } else if (chat_msg.type & FT_WALL){
            DBG(BLUE"CHAT_WALL"NONE" : You Have Wall_Mag Need Read!\n");
            show_message(NULL, NULL, chat_msg.msg, 1);
        } else if (chat_msg.type & FT_MSG) {
            DBG(BLUE"CHAT_TEAM"NONE" : You Have Team_Msg Need Read!\n");
            show_message(NULL, &user, chat_msg.msg, 0);
        } else if (chat_msg.type & FT_MAP) {
            show_message(NULL, NULL, "FootBall Game refresh", 1);
         //  parse_spirit(chat_msg.msg, chat_msg.size);

        } 
    }
}
