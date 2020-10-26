/*************************************************************************
	> File Name: client_recv.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 21时14分49秒
 ************************************************************************/

#include "head.h"
extern int sockfd;
void *client_recv(void *arg) {
    struct sockaddr_in server;
    struct FootBallMsg msg;
    socklen_t len = sizeof(server);
    int ret = recvfrom(sockfd, (void *)&msg, sizeof(msg), 0, (struct sockaddr *)&server, &len);
    if (ret != sizeof(msg)) {
        perror("client_recv()");
        exit(1);
    }
    if (msg.type & FT_HEART) {
        DBG(PINK"❤"NONE": Heart Beat From Server!\n");
        msg.type = FT_ACK;
        struct sockaddr_in server;
        socklen_t len = sizeof(server);
        sendto(sockfd, (void *)&msg, sizeof(msg), 0, (struct sockaddr *)&server, len);
    }

}
