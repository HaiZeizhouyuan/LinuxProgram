/*************************************************************************
	> File Name: fork_io_worker.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月12日 星期三 14时36分07秒
 ************************************************************************/

#include "head.h"

void worker(int fd, struct User *user) {
    while (1) {
        char buff[512] = {0};
        if ((recv(fd, buff, sizeof(buff), 0)) <= 0) {
            break;
        }
        printf("%s : %s\n", user->real_name, buff);
    }
    close(fd);
    DBG("%s Logout!\n", user->real_name);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port!\n", argv[0]);
        exit(1);
    }
    int server_listen, sockfd, port;
    port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        exit(1);
    }
    pid_t pid;
    while(1) {
        struct sockaddr_in client;
        struct User user;
        bzero(&client, sizeof(user));
        socklen_t len = sizeof(client);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
            perror("accept()");
            exit(1);
        }
        
        if ((pid = fork()) < 0) {
            perror("fork()");
            exit(1);
        }
        
        if (pid == 0) {
            worker(sockfd, &user);
            exit(0);
        }
    }
	return 0;
}
