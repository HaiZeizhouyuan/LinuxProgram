/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月20日 星期四 16时13分39秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
//extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
void *client_recv(void *arg) {
    struct ChatMsg msg;
    while (1) {
        bzero(&msg, sizeof(msg));
        if (recv(sockfd, (void *)&msg, sizeof(msg), 0) <= 0) {
            perror("client_recv()");
            exit(1);
        }
        DBG(BLUE"type : %d\n"NONE, msg.type);
        if(msg.type & CHAT_PUB) {
            DBG(GREEN"have chat_pub!\n"NONE);
            printf(BLUE"%s"NONE" : %s\n", msg.name, msg.msg);
            //print_message(sub_msg_win, &msg, 0);
        } else if (msg.type & CHAT_HEART) {
            DBG(RED"❤"NONE": heart beat from server!\n");
            msg.type = CHAT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & CHAT_PRI) {
            printf(BLUE"%s"GREEN"*"NONE": %s\n", msg.name, msg.msg);
            //print_message(sub_msg_win, &msg, 0);
        } else if (msg.type & CHAT_SYS) {
            printf(L_PINK"%s"NONE" :%s\n","Server Info", msg.msg);
            //print_message(sub_msg_win, &msg, 1);
        } else if (msg.type & (CHAT_FIN_1 | CHAT_FIN)) {
            printf(GREEN"Bye!\n"NONE);
           // sprintf(msg.msg, "Bye!");
           // print_message(sub_msg_win, &msg, 1);
            exit(1);
        }
    }
}
