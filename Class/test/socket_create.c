/*************************************************************************
	> File Name: socket_create.c
	> Author: zhouyuan  
	> Mail: 
	> Created Time: 2020年08月13日 星期四 19时00分19秒
 ************************************************************************/

#include "head.h"

int socket_create(int opt) {
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
        return = -1;
    }
    int val = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)val, sizeof(int)) < 0) {
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s__addr = inet_addr("0, 0, 0, 0");
    if (bind(socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    if (listen(sockfd,10) < 0) {
        return -1;
    }
    return sockfd;
}
