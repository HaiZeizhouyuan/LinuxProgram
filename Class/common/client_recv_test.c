/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月20日 星期四 16时13分39秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
extern char filename[20];
void *client_recv(void *arg) {
    struct ChatMsg msg;
    struct FileMsg fmsg;
    while (1) {
        bzero(&msg, sizeof(msg));
        if (recv(sockfd, (void *)&msg, sizeof(msg), 0) <= 0) {
            perror("client_recv()");
            exit(1);
        }
        if(msg.type & CHAT_PUB) {
            DBG(GREEN"have chat_pub!\n"NONE);
            printf(BLUE"%s"NONE" : %s\n", msg.name, msg.msg);
        } else if (msg.type & CHAT_HEART) {
            DBG(RED"❤"NONE": heart beat from server!\n");
            //msg.type = CHAT_ACK;
           // send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & CHAT_PRI) {
            DBG(RED"chat sb\n"NONE);
            printf(RED"%s"GREEN"*"NONE": %s\n", msg.name, msg.msg);
        } else if (msg.type & CHAT_SYS) {
            printf(L_PINK"%s"NONE" :%s %s\n","Server Info", msg.msg, msg.name);
        } else if (msg.type & (CHAT_FIN_1 | CHAT_FIN)) {
            printf(GREEN"Bye!\n"NONE);
            exit(1);
        } else if (msg.type & RECV_READY ) {
            DBG(RED"recv response start send %s!\n"NONE, filename);
            send_file(filename, sockfd);
            DBG(BLUE"have send finish %s!\n"NONE, filename);
        }
    }
}
