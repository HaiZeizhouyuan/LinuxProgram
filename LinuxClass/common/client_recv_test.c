/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月20日 星期四 16时13分39秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
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
            msg.type = CHAT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & CHAT_PRI) {
            DBG(RED"chat sb\n"NONE);
            printf(RED"%s"GREEN"*"NONE": %s\n", msg.name, msg.msg + 2 + strlen(msg.name));
        } else if (msg.type & CHAT_SYS) {
            printf(L_PINK"%s"NONE" :%s\n","Server Info", msg.msg);
        } else if (msg.type & (CHAT_FIN_1 | CHAT_FIN)) {
            printf(GREEN"Bye!\n"NONE);
            exit(1);
        } else if (msg.type & SEND_FILE) {
            DBG(GREEN"send_name : %s, recv_name: %s\n"NONE, msg.fmsg.send_name, msg.fmsg.recv_name);
            if (strncmp(msg.fmsg.send_name, msg.fmsg.recv_name, strlen(msg.fmsg.recv_name)) == 0) continue;
            DBG(BLUE"recv file!\n"NONE);
            FILE *fp;
            int fw;
            char *tmp = (char *)malloc(sizeof(char) * 1024);
            fp = fopen(msg.fmsg.filename, "r");
            if (fp != NULL) {
                int ans = 1;
                while(1) {
                    sprintf(tmp, "%s%d", msg.fmsg.filename, ans);
                    if ((fp = fopen(tmp, "r")) == NULL) {
                        memset(msg.fmsg.filename, 0, sizeof(msg.fmsg.filename));
                        strncpy(msg.fmsg.filename, tmp, strlen(tmp));
                        break;                 
                    }
                    memset(tmp, 0, sizeof(tmp));
                    ans += 1;       
                }     
            }
            free(tmp);
            fp = fopen(msg.fmsg.filename, "w");
            if ((fw = fwrite(msg.fmsg.buff, sizeof(msg.fmsg.buff), 1, fp)) < 0) {
                perror("fwrite()");
                exit(0);
            }
        }
    }
}
