/*************************************************************************
	> File Name: socet_connect.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月13日 星期四 19时52分50秒
 ************************************************************************/

#include "head.h"

int sock_connect() {
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM , 0)) < 0) {
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }
    return sockfd;
}
