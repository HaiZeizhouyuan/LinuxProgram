/*************************************************************************
	> File Name: server.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月23日 星期五 19时25分33秒
 ************************************************************************/
 #include "head.h"
#define MAX_TEAM 11
#define conf "./server_info.json"
int listener;
int red_mark[MAX_TEAM] = {0}, blue_mark[MAX_TEAM] = {0};
char msg[30] = {0}, data_stream[20] = {0};
int port = 0, msg_num = 0, red_num = 0, blue_num = 0;
int epoll_fd, repollfd, bepollfd;
struct LogResponse response;
struct LogRequest request;
struct Map court;
struct Bpoint ball;
struct BallStatus ball_status;
struct Score score;
struct User *red_team, *blue_team, *users;
WINDOW *Football, *Football_t, *Message, *Message_t, *Help, *Help_t, *Score, *Write;
pthread_mutex_t red_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t blue_mutex = PTHREAD_MUTEX_INITIALIZER;

//./a.out -p 6666
int main(int argc, char **argv) {
    int opt;
    pthread_t red_tid, blue_tid, heart_tid;
    setlocale(LC_ALL, "");
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
    argc -= (optind - 1);
    argv += (optind - 1);

    if (argc > 1) {
        fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
        exit(1);
    }
    
    if (!port) port = atoi(get_cjson_value(conf, "SERVERPORT"));
    court.width = atoi(get_cjson_value(conf, "COLS"));
    court.height = atoi(get_cjson_value(conf, "LINES"));
   /* struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl()");
        exit(1);
    }
    court.width = 4 * size.ws_col / 5 - 4;
    court.height = 4 * size.ws_row / 7 - 2;*/

    court.start.x = 3;
    court.start.y = 3;

    ball.x =  court.width / 2;
    ball.y =  court.height / 2;

    red_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));


    bzero(&ball_status, sizeof(ball_status));
    ball_status.by_team = -1;

    bzero(&score, sizeof(score));
    score.red = 0;
    score.blue = 0;
    
    DBG(BLUE"Get Port = %d seuccess!\n"NONE, port);

    if ((listener = socket_create_udp(port)) < 0) {
        //创建端口，　等待客户端的连接
        perror("socket_create_udp()");
        exit(1);
    }
    DBG(GREEN"server create scuuess...\n"NONE);

#ifndef _D
    initfootball();
#endif
    epoll_fd = epoll_create(2 * MAX_TEAM);//参数可忽略，大于零即可
    repollfd = epoll_create(MAX_TEAM);
    bepollfd = epoll_create(MAX_TEAM);

    if (epoll_fd < 0 || repollfd < 0 || bepollfd < 0) {
        //创建监控红队和蓝队的监控
        perror("epoll_create()");
        exit(1);
    }
    
    DBG(GREEN"epoll create success...\n"NONE);

    //创建红队和蓝队的队列．
    struct task_queue Red_Queue, Blue_Queue;

    task_queue_init(&Red_Queue, MAX_TEAM, repollfd);
    task_queue_init(&Blue_Queue, MAX_TEAM, bepollfd);
    
    DBG(GREEN"Red_Queue and Blue_Queue init success...\n"NONE);

    //创建红蓝线程处理红蓝队列信息
    pthread_create(&red_tid, NULL, sub_reactor, (void *)&Red_Queue);
    pthread_create(&blue_tid, NULL, sub_reactor, (void *)&Blue_Queue);
    pthread_create(&heart_tid, NULL, heart_beat, NULL);//处理心跳 

    DBG(BLUE"INFO"NONE": Thread poll created for worker!\n");
    signal(SIGINT, server_exit);

    struct epoll_event ev, events[MAX_TEAM * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev) < 0) {
       perror("epoll_ctl()");
       exit(1);
    }

	signal(SIGALRM, re_drew);


    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;//以后每一次执行的间隔时间
    itimer.it_value.tv_sec = 5;//第一次执行的时间
    itimer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, NULL);
    show_message(NULL, NULL, "Whiting for Login.", 1);
    sigset_t origmask, sigmask;
    sigemptyset(&sigmask);//清空sigmask信号集
    sigaddset(&sigmask, SIGALRM);//把SIGALRM加入到sigmask的信号集中, 即该位设为１,阻塞

    while(1) {
        DBG(GREEN"Epollfd Start Wait events!\n"NONE);
        pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
        int nfds = epoll_wait(epoll_fd, events, MAX_TEAM * 2, -1);
        DBG(BLUE"%d have %d events!\n"NONE, epoll_fd, nfds);
        pthread_sigmask(SIG_SETMASK, &origmask, NULL);
        if (nfds < 0) {
            perror("epoll_wait() failed");
            exit(1);
        }
        
    	for(int i = 0; i < nfds; i++) {
            struct User user;
            char buff[512] = {0};
            bzero(&user, sizeof(user));
            if (events[i].data.fd == listener) {
                //监测到有新用户    
                DBG(L_GREEN"Acceptor"NONE" : Accept a new client!\n");
                int new_fd = udp_accept(listener, &user);
                DBG(BLUE"new_fd is %d\n"NONE, new_fd);
                if (new_fd > 0) {
                    //是新用户，并且连接成功，开始插入
                    sprintf(buff, "Welcome %s Join Our Game, Team is %s\n", user.name, user.team ? "red" : "blue");
                    add_to_sub_reactor(&user);
                    show_message(NULL, NULL, buff, 1);
                  //  show_data_stream('l');
                    DBG(BLUE"Add %s to %s success!\n"NONE, user.name, user.team ? "Blue Team" : "Red Team");
                }
            } 
        }    
    }
    return 0;
}





