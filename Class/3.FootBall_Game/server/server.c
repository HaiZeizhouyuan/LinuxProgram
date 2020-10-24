/*************************************************************************
	> File Name: server.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月23日 星期五 19时25分33秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
#define conf "./server_info.json"
int server_listen;
char msg[30] = {0};
int port = 0;
struct LogResponse response;
struct LogRequest request;
struct User *red_team;
struct User *blue_team;
//./a.out -p 8888
int main(int argc, char **argv) {
    int opt;
    bzero(&response, sizeof(response));
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch(opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-p port]!\n", argv[0]);
                exit(1);          
        }
    }
    
    if (port == 0) port = atoi(get_cjson_value(conf, "SERVERPORT"));
    DBG(GREEN"Get Port = %d seuccess!\n"NONE, port);
    if ((server_listen = socket_create_udp(port)) < 0) {//创建端口，　等待客户端的连接
        perror("socket_create_udp()");
        exit(1);
    }
    red_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    
    struct epoll_event ev, events[MAX_TEAM * 2];
    int epoll_fd;
    if ((epoll_fd = epoll_create(MAX_TEAM * 2)) < 0) {
        perror("epoll_create()");
        exit(1);
    }
    ev.events = EPOLLIN;
    ev.data.fd = server_listen;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_listen, &ev) == -1) {
        perror("epoll_ctl()");
        exit(1);
    }
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);

    while(1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_TEAM * 2, -1);
        if (nfds == -1) {
            perror("epoll_event");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            recvfrom(events[i].data.fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);
            printf("msg : %s, Login : %s , %d\n", request.msg, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }
    }

    /*if (udp_connect(&client) < 0){
        perror("connect()");
        exit(1);
    }
    strcpy(response.msg, "Hi");
    response.type = 0;
    sendto(sockfd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);*/
    return 0;
}
