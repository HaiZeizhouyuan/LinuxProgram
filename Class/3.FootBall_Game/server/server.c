/*************************************************************************
	> File Name: server.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月23日 星期五 19时25分33秒
 ************************************************************************/
#include "head.h"
#define MAX_TEAM 11
#define THREADNUM 6
#define conf "./server_info.json"
int server_listen;
char msg[30] = {0};
int port = 0;
struct LogResponse response;
struct LogRequest request;
struct Map court;
struct Point start;
struct User *red_team, *blue_team, *users;
int epollfd, red_epollfd, blue_epollfd;
WINDOW *Football, *Football_t, *Message, *Help, *Score, *Write;
struct Bpoint ball;
struct BallStatus ball_status;
pthread_mutex_t red_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t blue_mutex = PTHREAD_MUTEX_INITIALIZER;

//./a.out -p 8888
int main(int argc, char **argv) {
    int opt;
    red_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    users = (struct User *)calloc(MAX_TEAM * 2, sizeof(struct User));
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
    court.width = atoi(get_cjson_value(conf, "COLS"));
    court.height = atoi(get_cjson_value(conf, "LINES"));

    court.start.x = 3;
    court.start.y = 3;

    ball.x = court.width / 2;
    ball.y = court.width / 2;
    bzero(&ball_status, sizeof(ball_status));
    DBG(BLUE"Get Port = %d seuccess!\n"NONE, port);

    if ((server_listen = socket_create_udp(port)) < 0) {
        //创建端口，　等待客户端的连接
        perror("socket_create_udp()");
        exit(1);
    }
    DBG(GREEN"server create scuuess...\n"NONE);

    epollfd = epoll_create(2 * MAX_TEAM);//参数可忽略，大于零即可
    red_epollfd = epoll_create(MAX_TEAM);
    blue_epollfd = epoll_create(MAX_TEAM);
    if (epollfd < 0 || red_epollfd < 0 || blue_epollfd < 0) {
        //创建监控红队和蓝队的监控
        perror("epoll_create()");
        exit(1);
    }
    
    DBG(GREEN"epoll create success...\n"NONE);
    
    //创建红队和蓝队的队列．
    struct task_queue Red_Queue;
    struct task_queue Blue_Queue;

    task_queue_init(&Red_Queue, MAX_TEAM, red_epollfd);
    task_queue_init(&Blue_Queue, MAX_TEAM, blue_epollfd);
    
    DBG(GREEN"Red_Queue and Blue_Queue init success...\n"NONE);

    //创建红蓝线程处理红蓝队列信息
    pthread_t red_tid, blue_tid, heart_t;
    pthread_create(&red_tid, NULL, sub_reactor, (void *)&Red_Queue);
    pthread_create(&blue_tid, NULL, sub_reactor, (void *)&Blue_Queue);

    DBG(BLUE"INFO"NONE": Thread poll created for worker!\n");
    
    struct epoll_event ev, events[MAX_TEAM * 2];
    ev.events = EPOLLIN;
    ev.data.fd = server_listen;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev);
    //initfootball();
    pthread_create(&heart_t, NULL, heart_beat, NULL);//处理心跳 
    signal(SIGINT, server_exit);
    while(1) {
        DBG(GREEN"Epollfd Start Wait events!\n"NONE);
        int nfds = epoll_wait(epollfd, events, MAX_TEAM * 2, -1);
        DBG(BLUE"%d have %d events!\n"NONE, epollfd, nfds);
        if (nfds < 0) {
            perror("epoll_wait()");
            exit(1);
        }
        
    	for(int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_listen) {
                //监测到有新用户    
                DBG(L_GREEN"Acceptor"NONE" : Accept a new client!\n");
                struct User user;
                bzero(&user, sizeof(user));
                int new_fd = udp_accept(server_listen, &user);
                DBG(BLUE"new_fd is %d\n"NONE, new_fd);
                if (new_fd > 0) {
                    //是新用户，并且连接成功，开始插入
                    printf("Welcome %s Join Our Game\n", user.name);
                    add_to_sub_reactor(&user);
                    DBG(BLUE"Add %s to %s success!\n"NONE, user.name, user.team ? "Blue Team" : "Red Team");
                }
            } 
        }
    }
    return 0;
}





