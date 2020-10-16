/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 16:23:05 2020
 ************************************************************************/

#include "work_list.h"

User user;
ChatMsg chatmsg;
int h_flag = 0,p_flag = 0,  n_flag = 0;
int sockfd;
char *tmp;
char inquire_num[5];
char root[10] = {"知忆"};
char have_sign[20];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//./client -h 47.94.34.228 -p 8888 -n zhouyuan
int main (int argc, char *argv[]) {
    int opt, port;
    if (argc >= 7) {
        while ((opt = getopt(argc, argv, "h:p:n:")) != -1) {
            switch(opt) {
                case 'h':
                    h_flag = 1;
                    strcpy(user.ip, optarg);
                    break;
                case 'p':
                    p_flag = 1;
                    port = atoi(optarg);
                    break;
                case 'n':
                    n_flag = 1;
                    strcpy(user.name, optarg);
                    break;
                default : 
                    fprintf(stderr, "Usage : %s -h IP -p PORT -n NMAE", argv[0]);
                    exit(1);
            }
        
        }
    } else if (argc == 4) {
        if ((tmp = get_conf("./user_information.txt","SERVERIP")) == NULL) {
            fprintf(stderr, "Error in get_ip!\n");
            exit(1);
        }
        strcpy(user.ip, tmp);
        if ((tmp = get_conf("./user_information.txt","SERVERPORT")) == NULL) {
            fprintf(stderr, "Error in get_port!\n");
            exit(1);
        }
        port = atoi(tmp);
        if ((tmp = get_conf("./user_information.txt", "SERVERNAME")) == NULL) {
            fprintf(stderr, "Error in get_name!\n");
            exit(1);
        }
        strcpy(user.name, tmp);
    } else {
        if (h_flag == 0) fprintf(stderr, "your no input your ip!\n");
        if (p_flag == 0) fprintf(stderr, "your no input your port!\n");
        if (n_flag == 0) fprintf(stderr, "your no input your name!\n");
        fprintf(stderr, "Usage %s -h ip -p port -n name\n", argv[0]);
        return 0;
    }

    chatmsg.type = 1;
    chatmsg.opt = 0000;

    DBG(BLUE"<Dbug>"NONE"name : %s, type : %d, opt : %d\n", chatmsg.name, chatmsg.type, chatmsg.opt);
    if ((sockfd = socket_connect(user.ip, port)) < 0){//建立客户端
        perror("sockfd_connet");
        exit(1);
    }
    strncpy(chatmsg.name, user.name, strlen(user.name));
    chatmsg.opt |= CHAT_SYN;
    DBG(RED"<Dbug>"NONE"opt = %d, chatmsg.name = %s\n", chatmsg.opt, chatmsg.name);
   // sprintf(chatmsg.msg, "welcome%s", user.name);
    if (send(sockfd, &chatmsg, sizeof(chatmsg), 0) < 0) {
        perror("send welcome faild");
        exit(1);
    }

    recv(sockfd, &chatmsg, sizeof(chatmsg), 0);
    printf("%s\n", chatmsg.msg);
    recv(sockfd, &chatmsg, sizeof(chatmsg), 0);
    printf("%s\n", chatmsg.msg);
    sprintf(have_sign, "%s have sign in!", user.name);
    DBG(GREEN"<Dbug>"NONE" : have_sign = %s, chatmsg.msg = %s, re = %d\n", have_sign, chatmsg.msg, strncmp(chatmsg.msg, have_sign, 5));
    if (strcmp(chatmsg.msg, have_sign) == 0) {
        exit(1);
    }
    DBG(GREEN"<Dbug>"NONE": have sign in!\n");
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) {
        while (1) {
            if (recv(sockfd, &chatmsg, sizeof(chatmsg), 0) > 0) {
                /*DBG(BLUE"<Dbug>"NONE":sys have kow!\n");
                if (chatmsg.opt & CHAT_FIN_1) {
                    DBG(BLUE"<DBUG>"NONE":sys have know logout!\n");
                    exit(0);
                } else {
                */
                    printf("%s\n", chatmsg.msg);
                //}
            }
        }
    } else {
        while(1) {

            memset(chatmsg.name, 0, strlen(chatmsg.name));
            memset(chatmsg.msg, 0, strlen(chatmsg.msg));
            DBG(YELLOW"<Dbg>"NONE" :have memset!\n");
            signal(SIGINT, Stop);
            DBG(BLUE"<Dbug>"NONE" : start input!\n");
            char buff[512] = {0};
            scanf("%[^\n]s", buff);
            getchar();
            DBG(PINK"<Dbug>"NONE":have scanf, msg : %s!\n", buff);
            strncpy(chatmsg.name, user.name, strlen(user.name));
            strncpy(chatmsg.msg, buff, strlen(buff));
            if (send(sockfd, &chatmsg, sizeof(chatmsg), 0) < 0) {
                perror("send()");
                exit(1);
            }
            DBG(YELLOW"<Dbug>"NONE" :have send msg!\n");
            /*recv(sockfd, &chatmsg, sizeof(chatmsg), 0);
            DBG(BLUE"<Dbug>"NONE" : have recv!\n");
            printf("dasda%s:%s\n", chatmsg.name, chatmsg.msg);
            DBG(RED"<Dbug>"NONE" :have printf ");*/
        }
    }
    return 0;
}

void Stop(int signal) {
    strncpy(chatmsg.name, user.name , strlen(user.name));
    chatmsg.opt |= CHAT_FIN;
    send(sockfd, &chatmsg, sizeof(chatmsg), 0);
    pthread_mutex_lock(&mutex);
    if (recv (sockfd, &chatmsg, sizeof(chatmsg), 0) > 0) {
        if (chatmsg.opt & CHAT_FIN_1) {
            DBG(BLUE"<DBUG>"NONE":sys have know logout!\n");
            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
}
void chat_msg_init(ChatMsg chatmsg ) {
    
}
