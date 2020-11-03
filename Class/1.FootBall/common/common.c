/*************************************************************************
	> File Name: common.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: Sat 28 Mar 2020 08:41:00 PM CST
 ************************************************************************/

#include "head.h"

void make_nonblock_ioctl(int fd){
    unsigned long ul = 1;
    ioctl(fd, FIONBIO, &ul);
}


void make_block_ioctl(int fd) {
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}


void setnonblocking(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl()");
        exit(1);
    }
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);//把新的flag 设置给fd文件
    return ;
}

void setblocking(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return ;
    }
    flag &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}


int socket_create_udp(int port) {
    int server_listen;

    if ((server_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    int val = 1;
    if (setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0 ) {
        return -1;
    }


    setnonblocking(server_listen);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;


    if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind()");
        return -1;
    }

    return server_listen;
}

int socket_udp() {
    int sockfd;//创建端口名
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;        
            
    }
    DBG(GREEN"Socket create success...\n"NONE);
    return sockfd;

}


