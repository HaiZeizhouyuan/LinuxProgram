/*************************************************************************
	> File Name: client.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月04日 星期四 19时50分33秒
 ************************************************************************/

#include "../common/head.h"

#define MAX_TEAM 11
int red_sockfd, blue_sockfd, sockfd = -1, server_port = 0, msg_num = 0;
char server_ip[20] = {0},  data_stream[20];
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


//signal
void logout(int signum) {
    struct FootBallMsg msg;
    msg.type = FT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    endwin();
    exit(1);
}


int main(int argc, char **argv)  {
    int opt;
    pthread_t recv_t, draw_t;
    struct LogRequest request;
    struct LogResponse response;
    bzero(&request, sizeof(request));
    bzero(&chat_msg, sizeof(struct FootBallMsg));
    bzero(&ctl_msg, sizeof(struct FootBallMsg));

    chat_msg.type = FT_MSG;
    ctl_msg.type = FT_CTL;

    while ((opt = getopt(argc, argv, "h:p:n:t:m:")) != -1) {
        switch (opt) {
            case 'h':
                strcpy(server_ip, optarg);
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'n':
                strcpy(request.name, optarg);
                break;
            case 't':
                request.team = atoi(optarg);
                break;
            case 'm':
                strcpy(request.msg, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-h host] [-p port]!\n", argv[0]);
                exit(1);
        }
    }

    argc -= (optind - 1);
    argv += (optind - 1);

    if (argc  > 1) {
        fprintf(stderr, "Usage : %s [-h host] [-p port]!\n", argv[0]);
        exit(1);
    }

    if (!server_port) server_port = atoi(get_cjson_value(conf, "SERVERPORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_cjson_value(conf, "SERVERIP"));
    if (!strlen(request.name)) strcpy(request.name, get_cjson_value(conf, "NAME"));
    if (!strlen(request.msg)) strcpy(request.msg, get_cjson_value(conf, "LOGMSG"));
    if (!request.team) request.team = atoi(get_cjson_value(conf, "TEAM"));

    court.width = atoi(get_cjson_value(conf, "COLS"));
    court.height = atoi(get_cjson_value(conf, "LINES"));
    court.start.x = 3;
    court.start.y = 3;

    ball.x = court.width / 2;
    ball.y = court.height / 2;

    signal(SIGINT, logout);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);
    
    socklen_t len = sizeof(server);

    DBG(GREEN"INFO"NONE" : server_ip = %s, server_port = %d, name = %s, team = %s logmsg = %s\n", server_ip, server_port, request.name, (request.team ? "BLUE" : "RED"), request.msg);
    if ((sockfd = socket_udp()) < 0) {
        perror("socket_udp");
        exit(1);
    }

    sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&server, len);

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    int retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);
    if (retval < 0) {
        perror("select");
        exit(1);
    } else if (retval) {
        int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&server, &len);
        if (ret != sizeof(response) || response.type) {
            printf("The Game Server refused your login.\n\tThis May be helpfull: %s\n", response.msg);
            exit(1);
        }
    } else {
        printf("The Game server is out of service.\n");
        exit(1);
    }
    
    show_message(NULL, NULL, response.msg, 1);
    connect(sockfd, (struct sockaddr *)&server, len);

#ifndef _D
    pthread_create(&draw_t, NULL, draw, NULL);
#endif
    pthread_create(&recv_t, NULL, client_recv, NULL);
   
    signal(SIGALRM, send_ctl);

    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 100000;

    setitimer(ITIMER_REAL, &itimer, NULL);

    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    while (1) {
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
                show_data_stream('j');
                show_message(NULL, &user, "stop football", 0);
                bzero(&chat_msg, sizeof(chat_msg));
                chat_msg.type = FT_CTL;
                chat_msg.ctl.action = ACTION_STOP;
                send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
                break;
            }
            case 'k': {
                show_data_stream('k');
                show_message(NULL, &user, "kick football", 0);
                bzero(&chat_msg, sizeof(chat_msg));
                chat_msg.type = FT_CTL;
                chat_msg.ctl.action = ACTION_KICK;
                chat_msg.ctl.strength = 1;
                send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
                break;
            }
            case 'l': {
                show_data_stream('l');
                show_message(NULL, &user, "carry football", 0);
                bzero(&chat_msg, sizeof(chat_msg));
                chat_msg.type = FT_CTL;
                chat_msg.ctl.action = ACTION_CARRY;
                chat_msg.ctl.strength = 1;
                send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
                break;
            }

            default:
                break;
        }
    }
    
    
    sleep(10);

    return 0;
}
