/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 16:12:25 2020
 ************************************************************************/

#include "head.h"

void *do_chat(void *arg) {
    int fd;
    fd = *(int *)arg;
    //解锁
    //int *fd = (int *)arg;
    while (1) {
        ssize_t nrecv;
        char buffer[512] = {0};
        if ((nrecv = recv(fd, buffer, sizeof(buffer), 0)) <= 0) {//接受信息
            close(fd);
            return NULL;
        }
        printf("CLI : %s\n", buffer);
        send(fd, buffer, strlen(buffer), 0);
    }
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port!\n", argv[0]);
        exit(1);
    }
    int listener, port;//listener端口，
    port = atoi(argv[1]);
    if ((listener = socket_create(port)) < 0) {//建立服务端
        perror("socket_create()");
        exit(1);
    }
    while (1) {
        int newfd;
        pthread_t tid;
        //加锁
        if ((newfd = accept(listener,NULL, NULL)) < 0) {
            perror("accept()");
            exit(1);
        }
        pthread_create(&tid, NULL, do_chat, (void *)&newfd);
        usleep(50);
    }

    return 0;
}


