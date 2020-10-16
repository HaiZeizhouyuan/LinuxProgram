/*************************************************************************
	> File Name: file_client.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月19日 星期六 10时22分22秒
 ************************************************************************/

#include "head.h"
//./a.out 47.94.34.228 8888 name
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s is port path_of_file!\n", argv[0]);
        return 1;
    }
    int sockfd, port;
    char name[1024] = {0};
    port = atoi(argv[2]);
    strcpy(name, argv[3]);
    if ((sockfd = socket_connect(argv[1], port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    send_file(name, sockfd);
    close(sockfd);
    return 0;
}
