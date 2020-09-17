/*************************************************************************
    > File Name: client:1.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月19日 星期三 15时19分28秒
 ************************************************************************/

#include "head.h"

char *conf = "./chat.conf";
char name[20] = {0};
char server_ip[20] = {0};
int sockfd, msg_num;

void logout(int signum) {
    DBG(RED"ctrl C!\n"NONE);
    struct ChatMsg msg;
    msg.type = CHAT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    if (recv(sockfd, (void *)&msg, sizeof(msg), 0) > 0) {
        printf(GREEN "Bye 1\n"NONE);
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
    if (!server_ip) strcpy(server_ip, get_conf(conf, "SERVERIP"));
    if (!strlen(name)) strcpy(name, get_conf(conf, "NAME"));

    DBG(GREEN"INFO"NONE" : server_ip = %s, server_port = %d, name = %s\n", server_ip, server_port, name);
    if ((sockfd = socket_connect_timeout(server_ip, server_port, 100000)) < 0) {
        perror("socket_connect()");
        exit(1);
    }

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
    strcpy(msg.name, name);

    while(1) {
        msg.type = CHAT_PUB;
        bzero(msg.msg, sizeof(msg.msg));
        //DBG(RED"start scanf!\n"NONE);
        scanf("%[^\n]s", msg.msg);
        getchar();
        //DBG(RED"have scanf!\n"NONE);
        if (msg.msg[0] == '@') {
            DBG(BLUE"@ sb\n"NONE);
            msg.type = CHAT_PRI;
        }
        if (msg.msg[0] == '#') msg.type = CHAT_FUNC;
        if (msg.msg[0] == '*') {
            msg.type = SEND_FILE;
            char recv_name[20];
            int i = 2;
            for (; i < 100; i++) {
                if (msg.msg[i] == ' ') break;
            }
            strncpy(msg.fmsg.filename, msg.msg + 2, i - 2);
            strncpy(msg.fmsg.send_name, name, strlen(name));
            strncpy(msg.fmsg.recv_name, msg.msg + i + 1, strlen(msg.msg) - i);
            send_file(msg.fmsg.filename, sockfd);
            continue;
        }
        int retval = send(sockfd, (void *)&msg, sizeof(msg), 0);      
        //DBG(RED"SEND"NONE" : %d bytes sent, %s\n", retval, msg.msg);
    }
	return 0;
}


