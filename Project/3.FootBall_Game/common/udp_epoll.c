/*************************************************************************
	> File Name: udp_epoll.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 23时20分37秒
 ************************************************************************/

#include "head.h"
extern int port, red_num, blue_num; //服务端的全局变量
extern struct User *red_team;
extern struct User *blue_team;
extern int repollfd, bepollfd;
extern pthread_mutex_t red_mutex, blue_mutex;
extern struct Bpoint ball;
extern WINDOW *Football;
extern struct Map court;
extern WINDOW *Football_t;
//将user添加到epollfd中, 监控user发生events事件
void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.events = events;//监控的事件
    ev.data.ptr = (void *)user;//监控的用户
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
    if (connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) < 0) {
        perror("connect()");
        return -1;    
    }
    DBG(GREEN"udp connect success!\n"NONE);
    return sockfd;

}


void add_new_loc(struct User *user) {
    if (user->team == 1) {
        red_num += 1;
        user->loc.x = court.width / 2 - 1 - 2 * (red_num / 6);
        user->loc.y = court.height / 2 - 3 + red_num % 6;
    } else {
        blue_num += 1;
        user->loc.x = court.width / 2 + 3 + 2 * (blue_num / 6);
        user->loc.y = court.height / 2 - 3 + blue_num % 6;
    }
}

//接受新用户
//user指针需要在调用之前创建一个User结构体，然后将其地址传入，这是一个输出参数
int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    struct LogRequest request;
    struct LogResponse response;
    socklen_t len = sizeof(client);

    char log_faile[30] = {"Login failed with Data errors!"};
    char log_success[30] = {"Login Success, Enjoy Yourself"};
    char re_log[30] = {"You Have login!\n"};

    //初始化必要的结构体
    bzero(&request,  sizeof(request));
    bzero(&response, sizeof(response));

    ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);

    if (ret != sizeof(request)) {
        response.type = 1;
        strcpy(response.msg, log_faile);
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        DBG(YELLOW"Recvfrom have problem, The recv size not wqual size of request!"NONE);
        return -1;
    }
    if (check_online(&request)) {
        response.type = 1;
        strcpy(response.msg, re_log);
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        DBG(RED"You Have Login!\n"NONE);
        return -1;
    }
    new_fd = udp_connect(&client); //是新用户建立连接
    if (fd == -1) {
        return -1;
    }
    strcpy(user->name, request.name);
    user->team = request.team;
    user->fd = new_fd;
    add_new_loc(user);
    wattron(Football_t, user->team ? COLOR_PAIR(6) : COLOR_PAIR(2));
    w_gotoxy_putc(Football_t, user->loc.x, user->loc.y, 'k');
    wattroff(Football_t, user->team ? COLOR_PAIR(6) : COLOR_PAIR(2));
    response.type = 0;
    strcpy(response.msg, log_success);
    sendto(user->fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
    show_message(NULL, user, request.msg, 0);
    DBG(BLUE"%s %s log success!\n"NONE, user->team ? "Blue Team" : "Red Team", user->name);
    return new_fd;
}

//在确定允许用户登录前，需要判断是否重复登录
int check_online(struct LogRequest *request) {
    for (int i = 0; i < MAX_TEAM; i++) {
        if(red_team[i].online && !strcmp(red_team[i].name, request->name)) return 1;
        if(blue_team[i].online && !strcmp(blue_team[i].name, request->name)) return 1;
    }
    return 0;
}

void add_to_sub_reactor(struct User *user) {
    struct User *team = (user->team ? blue_team : red_team);
    if (user->team) {
        pthread_mutex_lock(&blue_mutex);
    } else {
        pthread_mutex_lock(&red_mutex);
    }
    int loc;
    //找到空闲的位置
    for (int i = 0; i < MAX_TEAM; i++) {
        if (!team[i].online) {
            DBG(BLUE"Find loc at %d\n"NONE, i);
            loc = i;
            break;
        }
    }
    team[loc] = *user;
    team[loc].online = 1;
    team[loc].flag = 10;
    if (user->team) {
        pthread_mutex_unlock(&blue_mutex);
    } else {
        pthread_mutex_unlock(&red_mutex);
    }

    struct FootBallMsg msg;
    msg.type = FT_WALL;
    sprintf(msg.msg, "You good friend %s have login!\n", user->name);
    send_all(&msg);
    int user_epollfd = (user->team ? bepollfd : repollfd);
    if (user->team) add_event_ptr(bepollfd, team[loc].fd, EPOLLIN | EPOLLET, &team[loc]);
    else add_event_ptr(repollfd, team[loc].fd, EPOLLIN | EPOLLET, &team[loc]);
    DBG(BLUE"%s have insert %d success!, loc is %d\n"NONE, user->name, user->team, loc);
}


