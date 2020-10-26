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
char filename[20] = {0};
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
    struct FileMsg filemsg;
    while(1) {
        msg.type = CHAT_PUB;
        bzero(msg.msg, sizeof(msg.msg));
        scanf("%[^\n]s", msg.msg);
        getchar();
        if (msg.msg[0] == '@') {
            DBG(BLUE"@ sb\n"NONE);
            msg.type = CHAT_PRI;
        }
        if (msg.msg[0] == '#') {
            msg.type = CHAT_FUNC;
        }
        //send file sb;

        if (strncmp(msg.msg, "send", 4) == 0) {
            pid_t pid;

            int black_flag = 0, file_loc, name_loc;
            for (int i = 0; i < strlen(msg.msg); i++) {
                if (msg.msg[i] == ' ') {
                    black_flag += 1;
                    if (black_flag == 1) file_loc = i;
                    if (black_flag == 2) name_loc = i;
                }
            }
            DBG(RED"msg: %s, file_loc = %d, name_loc = %d\n"NONE, msg.msg, file_loc, name_loc);
            if (black_flag == 0) {//send;
                printf("The format of your send file is wrong\n");
                continue;
            } else if (black_flag == 1) { //send file
                DBG(BLUE"send file to all\n"NONE);
                msg.type = SEND_FILE_ALL;
                strcpy(filemsg.name, msg.msg + file_loc + 1);
                strcpy(filemsg.recv_name, "NULL");
            } else { //send file name
                DBG(BLUE"send file to sb\n"NONE);
                msg.type = SEND_FILE_TO;
                strncpy(filemsg.name, msg.msg + file_loc + 1, name_loc - file_loc - 1);
                strcpy(filemsg.recv_name, msg.msg + name_loc + 1);
            }
            strcpy(filename, filemsg.name);
            DBG(RED"Filename is %s, recv_name is %s\n"NONE, filemsg.name, filemsg.recv_name);
            send(sockfd, (void *)&msg, sizeof(msg), 0);
            send(sockfd, (void *)&filemsg, sizeof(filemsg), 0);
            bzero(&msg, sizeof(msg));
            recv(sockfd, (void *)&msg, sizeof(msg), 0);
            if (msg.type & RECV_END) {
                DBG(GREEN"FILE:"NONE" : type : %d, filename : %s, recv_name : %s!\n", msg.type, filemsg.name, filemsg.recv_name);
                printf("File have send finish!\n");
                continue;
            } 
        }
        
        int retval = send(sockfd, (void *)&msg, sizeof(msg), 0);  
        
        //DBG(RED"SEND"NONE" : %d bytes sent, %s\n", retval, msg.msg);
    }
	return 0;
}


