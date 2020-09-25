/*************************************************************************
	> File Name: one_thread.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月12日 星期三 14时37分48秒
 ************************************************************************/

#include "head.h"

//./a.out port 
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port !\n", argv[0]);
        exit(1);
    }

    int server_listen, sockfd, port;
    port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {//建立服务端
        perror("socket_create()");
        exit(1);
    }

    while (1) {
        struct sockaddr_in client;
        char buff[512] = {0};
        socklen_t len = sizeof(client);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
            printf("accept() have bug");
            perror("accept()");
            exit(1);
        }
        read(sockfd, buff, sizeof(buff));
        printf(BLUE"CLI"NONE ":%s\n", buff);
        close(sockfd);
    }

	return 0;
}
