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
        if (ret <= 0) {
            return 0;
        }
        user.team = chat_msg.team;
        strcpy(user.name, chat_msg.name);
        if (chat_msg.type & FT_HEART) {//心跳
            chat_msg.type = FT_ACK;
            send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
        } else if (chat_msg.type & FT_FIN) {//服务端下线
            show_message(NULL, NULL, "Server Have Logout!\n", 1);
            close(sockfd);
            endwin();
            exit(0);
        } else if (chat_msg.type & FT_WALL){
            show_message(NULL, NULL, chat_msg.msg, 1);
        } else if (chat_msg.type & FT_MSG) {
            show_message(NULL, &user, chat_msg.msg, 0);
        } else if (chat_msg.type & FT_PRI) {
            show_message(NULL, &user, chat_msg.msg, 2);
        } else if (chat_msg.type & FT_MAP) {
            parse_spirit(chat_msg.msg, chat_msg.size);
            client_re_draw();
        } 
    }
}
