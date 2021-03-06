/*************************************************************************
    > File Name: client:1.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月19日 星期三 15时19分28秒
 ************************************************************************/

#include "head.h"
WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;

char *conf = "./chat.conf";
char name[20] = {0};
char server_ip[20] = {0};
int sockfd, msg_num;
void logout(int signum) {
    DBG(RED"ctrl C!\n"NONE);
    struct ChatMsg msg;
    msg.type = CHAT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    if (recv(sockfd, (void *)&msg, sizeof(msg), 0) <= 0) {
        sprintf(msg.msg, "Bye");
        print_message(sub_msg_win, &msg, 1);
        sleep(1);
        endwin();
        exit(1);
    }
}
int main(int argc, char **argv) {
    int opt, server_port = 0;
    pthread_t recv_t;
    while ((opt = getopt(argc, argv, "p:h:n:")) != -1) {
        switch(opt) {
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'n':
                strcpy(name, optarg);
                break;
            case 'h':
                strcpy(server_ip, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port -n name -h host\n", argv[0]);
                exit(1);
        }
    }
    if (!server_port) server_port = atoi(get_conf(conf, "SERVERPORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_conf(conf, "SERVERIP"));
    if (!strlen(name)) strcpy(name, get_conf(conf, "NAME"));

    DBG(GREEN"INFO"NONE" : server_ip = %s, server_port = %d, name = %s\n", server_ip, server_port, name);
    if ((sockfd = socket_connect_timeout(server_ip, server_port, 100000)) < 0) {
        perror("socket_connect()");
        exit(1);
    }

    setlocale(LC_ALL, "");

    DBG(GREEN"INFO"NONE" : connected to server via TCP\n");
    struct ChatMsg msg;
    bzero(&msg, sizeof(msg));
    strcpy(msg.name, name);
    msg.type = CHAT_SYN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    DBG(GREEN"INFO"NONE" : sent CHAT_SYN to server\n");
    bzero(&msg, sizeof(msg));

    recv(sockfd, (void *)&msg, sizeof(msg), 0);
    DBG(GREEN"INFO"NONE":recved packet from server\n");
    if (msg.type & CHAT_NAK) {
        DBG(RED"Server returned Error"NONE" : login failed : %s\n", msg.msg);
        exit(1);
    }
    DBG(RED"login success"NONE" : %s\n", name);
    signal(SIGINT, logout);//如果^c则发送信号执行函数
    pthread_create(&recv_t, NULL, client_recv, NULL);

    init_ui();
    strcpy(msg.name, name);
    echo();
    nocbreak();//打开行缓冲
    while(1) {
        msg.type = CHAT_PUB;
        bzero(msg.msg, sizeof(msg.msg));  
        w_gotoxy_puts(sub_input_win, 1, 1, "Input :");
        mvwscanw(sub_input_win, 2, 2, "%[^\n]s", msg.msg);
        if (!strlen(msg.msg)) {
            wclear(input_win);
            box(input_win, 0, 0);
            wrefresh(input_win);
            continue;
        }
        if (msg.msg[0] == '@') {
            DBG(BLUE"@ sb\n"NONE);
            msg.type = CHAT_PRI;
        }
        if (msg.msg[0] == '#') {
            msg.type = CHAT_FUNC;
        }
        int retval = send(sockfd, (void *)&msg, sizeof(msg), 0);  
        wclear(input_win);
        box(input_win, 0, 0);
        wrefresh(input_win);
    }
    sleep(10);
	return 0;
}


