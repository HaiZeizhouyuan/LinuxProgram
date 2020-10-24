/*************************************************************************
	> File Name: udp_epoll.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 23时20分37秒
 ************************************************************************/

#include "head.h"
//将user添加到epollfd中, 监控user发生events事件
void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = (void *)user;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl_add()");
        exit(1);
    }
    return ;
}

void del_event(int epollfd, int fd) {
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epell_ctl_del()");
        exit(1);
    }
    return ;
}
//连接客户端
int udp_connect(struct sockaddr_in *client) {
    int sockfd;
    if ((sockfd = socket_create_udp(port)) < 0) {
        perror("socket_create_udp()");
        return -1;
    }
    if (connect(sockfd, (struct sockaddr *)&client, sizeof(struct sockaddr_in)) < 0) {
        return -1;    
    }
    return sockfd;

}

//接受新用户
int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    struct LogRequest request;
    struct LogResponse response;
    socklen_t len = sizeof(client);
    char log_faile[30] = {"Login failed with Data errors!"};
    char log_success[30] = {"Login Success, Enjoy Yourself"};
    char re_log[30] = {"You Have login!\n"};
    bzero(&request,  sizeof(request));
    bzero(&response, sizeof(response));

    ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);
    if (ret != sizeof(request)) {
        response.type = 1;
        strcpy(response.msg, log_faile);
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }
    if (check_online(&request)) {
        response.type = 1;
        strcpy(response.msg, re_log);
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }
    new_fd = udp_connect(&client);
    if (fd == -1) {
        return -1;
    }
    strcpy(user->name, request.name);
    user->team = request.team;
    user->fd = fd;
    user->online = 1;
    strcpy(user->name, request.name);
    user->flag = 10;

    response.type = 0;
    strcpy(response.msg, log_success);
    sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
    return new_fd;
}

int check_online(struct LogRequest *request) {
    for (int i = 0; i < MAX; i++) {
        if(team_red[i].online && !strcmp(team_red[i].name, request->name)) return 1;
        if(team_blue[i].online && !strcmp(team_blue[i].name, request->name)) return -1;

    }
    return 0;
}
void add_to_sub_reactor(struct User *user) {
    struct User *team = (user->team ? blue_team : red_team);
    int loc;
    for (int i = 0; i < MAX_TEAM; i++) {
        if (!team[i].online) {
            loc = i;
            break;
        }
    }
    team[loc] = *user;
    int user_sockfd = (user->team ? blue_sockfd : red_sockfd);
    add_event_ptr(user_sockfd, user->fd, EPOLLIN | EPOLLET, user);
    DBG(BLUE"%s have insert %d success!\n"NONE, user->name, user->team);
}


