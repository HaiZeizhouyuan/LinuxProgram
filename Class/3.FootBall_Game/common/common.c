/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  8 14:32:12 2020
 ************************************************************************/

#include "head.h"

void setnonblocking(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl()");
        exit(1);
    }
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);//把新的flag设置给fd文件
    return ;
}

void setblocking(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl()");
        exit(1);
    }
    flag &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
    return ;
}

  
int socket_create_udp(int port) {
    //创建套接字
    int server_listen;
    if ((server_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    //重启地址
    int val = 1;
    if (setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR,(const char*)&val, sizeof(int)) < 0) {
        return -1;    
    }

    setnonblocking(server_listen);//设为非阻塞套接字
    struct sockaddr_in server; //创建表单名server;
    server.sin_family = AF_INET; //协议族
    server.sin_port = htons(port);//本地字节序转换成网络字节序的短整形
    server.sin_addr.s_addr = INADDR_ANY; //所有的ip都要
    if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {//绑定ip和端口
        perror("bind()");
        return -1;
    }
    //setblocking(server_listen);
    setnonblocking(server_listen);
    DBG(GREEN"Server socket create success...\n"NONE);
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
