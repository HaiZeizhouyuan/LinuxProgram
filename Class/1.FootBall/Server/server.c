/*************************************************************************
	> File Name: server.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月02日 星期二 18时12分01秒
 ************************************************************************/
#include "head.h"

#define MAX_TEAM 11
#define conf "./server_info.json"
int listener;
int red_mark[MAX_TEAM] = {0}, blue_mark[MAX_TEAM] = {0};
char msg[30] = {0}, data_stream[20] = {0};
int port = 0, msg_num = 0;
int epoll_fd, repollfd, bepollfd, help_num = 0;
struct LogResponse response;
struct LogRequest request;
struct Map court;
struct Bpoint ball;
struct BallStatus ball_status;
struct Score score;
struct User *rteam, *bteam, *users;
WINDOW *Football, *Football_t, *Message, *Message_t, *Help, *Help_t, *Score, *Write;
pthread_mutex_t red_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t blue_mutex = PTHREAD_MUTEX_INITIALIZER;

//a.out -p 6666
int main(int argc, char **argv) {
    int opt;
    pthread_t draw_t, red_t, blue_t, heart_t;
    setlocale(LC_ALL, "");
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
                exit(1);
        }
    }
    argc -= (optind - 1);
    argv += (optind - 1);

    if (argc > 1) {
        fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
        exit(1);
    }

    if (!port) port = atoi(get_cjson_value(conf, "SERVERPORT"));
    court.width = atoi(get_cjson_value(conf, "COLS"));
    court.height = atoi(get_cjson_value(conf, "LINES"));

    court.start.x = 3;
    court.start.y = 3;

    ball.x = court.width / 2;
    ball.y = court.height / 2;

    rteam = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    bteam = (struct User *)calloc(MAX_TEAM, sizeof(struct User));

    bzero(&ball_status, sizeof(ball_status));
    ball_status.by_team= -1;
    bzero(&score, sizeof(score));

    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

    DBG(GREEN"INFO"NONE" : Server start on Port %d\n", port);
#ifndef _D
    pthread_create(&draw_t, NULL, draw, NULL);
    //initfootball();
#endif
    epoll_fd = epoll_create(MAX_TEAM * 2);
    repollfd = epoll_create(MAX_TEAM);
    bepollfd = epoll_create(MAX_TEAM);

    if (epoll_fd < 0 || bepollfd < 0 || repollfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    struct task_queue redQueue;
    struct task_queue blueQueue;

    task_queue_init(&redQueue, MAX_TEAM, repollfd);
    task_queue_init(&blueQueue, MAX_TEAM, bepollfd);

    pthread_create(&red_t, NULL, sub_reactor, (void *)&redQueue);
    pthread_create(&blue_t, NULL, sub_reactor, (void *)&blueQueue);
    pthread_create(&heart_t, NULL, heart_beat, NULL);

    signal(SIGINT, server_exit);

    struct epoll_event ev, events[MAX_TEAM * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        perror("epoll_ctl()");
        exit(1);
    }
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    signal(SIGALRM, re_drew);
    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;
    itimer.it_value.tv_sec = 5;
    itimer.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &itimer, NULL);

    show_message( NULL, NULL, "Waiting for Login.", 1);
    sigset_t origmask, sigmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGALRM);

    while (1) {
        DBG(YELLOW"Main Thread"NONE" :  before epoll_wait\n");
        pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
        int nfds = epoll_wait(epoll_fd, events, MAX_TEAM * 2, -1);    
        pthread_sigmask(SIG_SETMASK, &origmask, NULL);
        DBG(YELLOW"Main Thread"NONE" :  After epoll_wait\n");
        if (nfds < 0) {
            perror("epoll_wait()");
            exit(1);
        }
            
        for (int i = 0; i < nfds; i++) {
            struct User user;
            char buff[512] = {0};
            DBG(YELLOW"EPOLL"NONE" :  Doing with %dth fd\n", i);
            if (events[i].data.fd == listener) {
                int new_fd = udp_accept(listener, &user);
                if (new_fd > 0) {
                    sprintf(buff, "%s Login the Game.", user.name);
                    DBG(YELLOW"EPOLL"NONE" :Add %s to %s sub_reactor.\n", user.name, (user.team ? "BLUE" : "RED"));
                    add_to_sub_reactor(&user);
                    show_data_stream('l');
                    show_message(NULL, NULL, buff, 1);
                }
            } 
            /*else {
                recv(events[i].data.fd, buff, sizeof(buff), 0);
                printf(PINK"RECV"NONE" : %s\n", buff);
            }
            char info[1024] = {0};
            w_gotoxy_puts(Message, 1, 2, info);*/
        }
    }
    return 0;
}


