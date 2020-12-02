/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月20日 星期四 16时13分39秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;

void *client_recv(void *arg) {
    struct ChatMsg msg;
    while (1) {
        bzero(&msg, sizeof(msg));
        if (recv(sockfd, (void *)&msg, sizeof(msg), 0) <= 0) {
            perror("client_recv()");
            exit(1);
        }
        if(msg.type & CHAT_PUB) {
            print_message(sub_msg_win, &msg, 0);
        } else if (msg.type & CHAT_HEART) {
            msg.type = CHAT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & CHAT_PRI) {
            print_message(sub_msg_win, &msg, 0);
        } else if (msg.type & CHAT_SYS) {
            print_message(sub_msg_win, &msg, 1);
        } else if (msg.type & (CHAT_FIN_1 | CHAT_FIN)) {
            sprintf(msg.msg, "Bye!");
            print_message(sub_msg_win, &msg, 1);
            endwin();
            exit(1);
        } 
    }
}
