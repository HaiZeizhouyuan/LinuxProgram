#include "head.h"
#define MAX 11
#define conf "./server_info.json"
int server_listen;
int red_mark[MAX_TEAM] = {0}, blue_mark[MAX_TEAM] = {0};
char msg[30] = {0}, data_stream[20] = {0};
int port = 0, msg_num = 0;
int epollfd, red_epollfd, blue_epollfd, help_num = 0;
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

int main(int argc, char **argv) {
	setlocale(LC_ALL,"");
	int opt, listener, epoll_fd;
    pthread_t draw_t, red_t, blue_t, heart_t;
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

    if (!port) port = atoi(get_cjson_value(conf, "PORT"));
    //port = atoi(get_cjson_value(conf, "DATAPORT"));
    court.width = atoi(get_cjson_value(conf, "COLS"));
    court.height = atoi(get_cjson_value(conf, "LINES"));
    court.start.x = 3;
    court.start.y = 3;
	ball.x = court.width / 2;
	ball.y = court.height / 2;

    red_team = (struct User *)calloc(MAX, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX, sizeof(struct User));

    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

    DBG(GREEN"INFO"NONE" : Server start on Port %d\n", port);
#ifndef _D
    initfootball();
#endif

    epoll_fd = epoll_create(MAX * 2);
    red_epollfd = epoll_create(MAX);
    blue_epollfd = epoll_create(MAX);

    if (epoll_fd < 0 || red_epollfd < 0 || blue_epollfd < 0) {
        perror("epoll_create");
        exit(1);
    }

	struct task_queue redQueue;
	struct task_queue blueQueue;

	task_queue_init(&redQueue, MAX, red_epollfd);
	task_queue_init(&blueQueue, MAX, blue_epollfd);

	pthread_create(&red_t, NULL, sub_reactor, (void *)&redQueue);
	pthread_create(&blue_t, NULL, sub_reactor, (void *)&blueQueue);
	pthread_create(&heart_t, NULL, heart_beat, NULL);
		
	signal(SIGINT, server_exit);

    struct epoll_event ev, events[MAX * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev);
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    
	signal(SIGALRM, re_drew);
	
	struct itimerval itimer;
	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_usec = 50000;
	itimer.it_value.tv_sec = 5;
	itimer.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itimer, NULL);

	show_message(NULL,NULL , "Waiting for Login", 1);

    sigset_t origmask, sigmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGALRM);
    while (1) {
		DBG(YELLOW"Main Pthread"NONE" :  before epoll_wait\n");
        pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
        int nfds = epoll_wait(epoll_fd, events, MAX * 2, -1);
        pthread_sigmask(SIG_SETMASK, &origmask, NULL);
           
        for (int i = 0; i < nfds; i++) {
			struct User user;
            char buff[512] = {0};
            DBG(YELLOW"EPOLL"NONE" :  Doing with %dth fd\n", i);
            if (events[i].data.fd == listener) {
                //accept();
                int new_fd = udp_accept(listener, &user);
				if (new_fd > 0) {
					DBG(YELLOW"Main thread"NONE" : Add %s to %s sub_reactor.\n", user.name, (user.team ? "BLUE": "RED"));
					add_to_sub_reactor(&user);
					//show_data_stream('l');
					sprintf(buff, "%s login the Game.", user.name);
					show_message( NULL, NULL, buff, 1);
				}
            } else {
                recv(events[i].data.fd, buff, sizeof(buff), 0);
                DBG(PINK"RECV"NONE" : %s\n", buff);
            }
            //char info[1024] = {0};
            //w_gotoxy_puts(Message, 1, 2, info);
        }

    }
 return 0;
}
