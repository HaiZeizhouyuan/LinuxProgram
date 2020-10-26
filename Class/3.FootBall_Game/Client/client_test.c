/*************************************************************************
	> File Name: client.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 19时46分33秒
 ************************************************************************/

#include "head.h"
int sockfd = -1, team = -1, server_port = 0;
char server_ip[20] = {0};
char chat_msg[512] = {0};

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s ip port!\n", argv[0]);
        exit(1);
    }
    server_port = atoi(argv[2]);
    strncpy(server_ip, argv[1], 20);

    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);
    if ((sockfd = socket_udp()) < 0) {
        perror("socket_udp");
        exit(1);
    }
    DBG(GREEN"socket_udp create success!\n"NONE);

    if (connect(sockfd, (struct sockaddr *)&server, len) < 0) {
        perror("connect");
        exit(1);
    }

    DBG(GREEN"connect success!\n"NONE);
    sendto(sockfd, "Hi", sizeof("hi"), 0, (struct sockaddr *)&server, len);
    sleep(1);
    recvfrom(sockfd, chat_msg, sizeof(chat_msg), 0, (struct sockaddr *)&server, &len);
    printf("recv from server : %s \n", chat_msg);
    return 0;
}
