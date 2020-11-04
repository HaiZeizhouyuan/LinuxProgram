/*************************************************************************
	> File Name: client.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 19时46分33秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
int red_sockfd, blue_sockfd, sockfd = -1, team = -1, server_port = 0, msg_num = 0;
char server_ip[20] = {0},  name[20] = {0}, log_msg[20] = {0}, data_stream[20];
char *conf = "./football.json";

WINDOW *Write, *Message, *Message_t,  *Help, *Help_t, *Score, *Football, *Football_t;

struct User *blue_team, *red_team;
struct Score score;
struct Bpoint ball;
struct BallStatus ball_status;
struct Map court;
struct Point start;
struct FootBallMsg chat_msg;
struct FootBallMsg ctl_msg;
struct LogRequest request;
struct LogResponse response;
struct User user;

int main(int argc, char **argv) {
    setlocale(LC_ALL,"");
    int opt;
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));
    bzero(&ctl_msg, sizeof(ctl_msg));
    bzero(&chat_msg, sizeof(chat_msg));

    while ((opt = getopt(argc, argv, "h:p:t:m:n:")) != -1) {
        switch(opt) {
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'h':
                strcpy(server_ip, optarg);
                break;
            case 'n':
                strcpy(name, optarg);
                break;
            case 't':
                team = atoi(optarg);
                break;
            case 'm':
                strcpy(log_msg, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-hptmn]!\n", argv[0]);
                exit(1);
        }
    }
    

    if (server_port == 0) server_port = atoi(get_cjson_value(conf, "SERVERPORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_cjson_value(conf, "SERVERIP"));
    if (!strlen(name)) strcpy(name, get_cjson_value(conf, "NAME"));
    if (team == - 1) team = atoi(get_cjson_value(conf, "TEAM"));
    if (strlen(log_msg) == 0) strcpy(log_msg, get_cjson_value(conf, "LOGMSG"));

   // court.width = atoi(get_cjson_value(conf, "COLS"));
   //court.height = atoi(get_cjson_value(conf, "LINES"));
    DBG(GREEN"Get server_port = %d, server_ip = %s, name = %s, team = %d, LOGMSG = %s  success!\n"NONE, server_port, server_ip, name, team, log_msg);
    
    ctl_msg.type = FT_CTL;
    strcpy(chat_msg.name, name);
    chat_msg.team = team;
    chat_msg.type = FT_MSG;

    strcpy(user.name, name);
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl()");
        exit(1);           
    }

    court.width = 4 * size.ws_col / 5 - 4;
    court.height = 4 * size.ws_row / 7 - 2;
    
    court.start.x = 3;
    court.start.y = 3;

    ball.x = court.width / 2;
    ball.y = court.height/ 2;
    bzero(&ball_status, sizeof(ball_status));

    signal(SIGINT, client_exit);
    red_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);
    socklen_t len = sizeof(server);
    if ((sockfd = socket_udp()) < 0){
        perror("socket_udp()");
        exit(1);
    }
    
    DBG(GREEN"client create success...\n"NONE);
    strcpy(request.msg, log_msg);
    strcpy(request.name, name);
    request.team = team;
    sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&server, len);

    fd_set wfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    //设置超时时间为５秒，0毫秒
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);

    if (retval < 0) { //出错
        perror("select()");
        exit(1);
    } else if (retval) {
        int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0,(struct sockaddr *)&server, &len);
        if (ret != sizeof(response) || response.type) { //response.type == 1服务端是拒绝连接
            printf("The Game Server refused your logion.\n\tThis May be helpful:%s\n", response.msg);
            exit(1);
        }
    } else {//超时
        printf("The Game Server is out of service!.\n");
        exit(1);
    }
    printf("Server : %s\n", response.msg);
    connect(sockfd, (struct sockaddr *)&server, len);
    show_data_stream('l');
    pthread_t recv_t, draw_t;
#ifndef _D
    initfootball();
#endif
    pthread_create(&recv_t, NULL, client_recv, NULL);
    
    signal(SIGALRM, send_ctl);//由setitimer触发的信号
    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 100000;
    setitimer(ITIMER_REAL, &itimer, NULL );

    noecho();
    cbreak;
    keypad(stdscr, TRUE);

    while(1) {
        int c = getchar();
        switch(c) { 
            case 'a':
                ctl_msg.ctl.dirx -= 1;
                break;
            case 'd':
                ctl_msg.ctl.dirx += 1;
                break;
            case 'w':
                ctl_msg.ctl.diry -= 1;
                break;
            case 's':
                ctl_msg.ctl.diry += 1;
                break;
            case 13:
                send_chat();
                break;
            case ' ':
                show_strength();
                break;
            case 'j': {
                show_data_stream('s');
                show_message(NULL, &user, "stop football", 0);
                bzero(&ctl_msg, sizeof(ctl_msg));
                ctl_msg.type = FT_CTL;
                ctl_msg.ctl.action = ACTION_STOP;
                send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
                break;
            }
            case 'k': {
                show_data_stream('k');
                show_message(NULL, &user, "kick football", 0);
                bzero(&ctl_msg, sizeof(ctl_msg));
                ctl_msg.type = FT_CTL;
                ctl_msg.ctl.action = ACTION_KICK;
                ctl_msg.ctl.strength = 1;
                send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
                break;
            }
            case 'l': {
                show_data_stream('c');
                show_message(NULL, &user, "carry football", 0);
                bzero(&ctl_msg, sizeof(ctl_msg));
                ctl_msg.type = FT_CTL;
                ctl_msg.ctl.action = ACTION_CARRY;
                ctl_msg.ctl.strength = 1;
                send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
                break;
            }
            default:
                break;
        }
    }
    sleep(10);
    
    return 0;
}
