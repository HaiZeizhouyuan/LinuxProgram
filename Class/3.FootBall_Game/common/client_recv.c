/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 21时14分49秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
void *client_recv(void *arg) {
    struct FootBallMsg msg;
    while(1) {
        bzero(&msg, sizeof(msg));
        DBG(GREEN"Wait Msg!\n"NONE);
        int ret = recv(sockfd, (void *)&msg, sizeof(msg), 0);
        DBG(RED"ret = %d\n"NONE, ret);
        if (ret != sizeof(msg)) {
            perror("client_recv()");
            exit(1);
        }
        DBG(BLUE"Have msg recv!\n"NONE);
        if (msg.type & FT_HEART) {
            DBG(PINK"❤"NONE": Heart Beat From Server!\n");
        } else if (msg.type & FT_FIN) {
            DBG(GREEN"FIN"NONE" : Server Have Logout!\n");
            printf("Server Have Logout!\n");
            exit(0);
        } else if (msg.type & FT_WALL){
            DBG(BLUE"CHAT_WALL"NONE" : You Have Wall_Mag Need Read!\n");
            if (msg.team == 1) printf("<WHOLE>" BLUE"%s"NONE ":%s\n", msg.name, msg.msg);
            else printf("<WHOLE>" RED"%s"NONE" : %s\n", msg.name, msg.msg);
        } else if (msg.type & FT_MSG) {
            DBG(BLUE"CHAT_TEAM"NONE" : You Have Team_Msg Need Read!\n");
            if (msg.team == 1) printf(BLUE"%s"NONE" : %s\n", msg.name, msg.msg);
            else printf(RED"%s"NONE" : %s\n", msg.name, msg.msg);
        }


    }
}
