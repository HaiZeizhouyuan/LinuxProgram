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
//./client -h 47.94.34.228 -p 8888 -n zhouyuan
int main (int argc, char *argv[]) {
    int opt, port;
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
   
    /*if (argc == 4) {
        FILE fd = NULL;
        if ((fd = fopen("information.txt", r) == NULL)) {
            perror("fopen()");
            exit(1);
        }
        ssize_t fr;
        char buff[1024];
        while ((fr = getline(buff, strlen(buff),fd)) < 0) {
            
        }

    }*/
    if (argc != 7) {
        fprintf(stderr, "Usage %s ip port\n", argv[0]);
        exit(1);
    }

    int sockfd;

    if ((sockfd = socket_connect(user.ip, port)) < 0){//建立客户端
        perror("sockfd_connet");
        exit(1);
    }
    char welcome[30];
    sprintf(welcome, "Welcome:%s", user.name);
    if (send(sockfd, welcome, strlen(welcome), 0) < 0) {
        perror("sendname faild");
        exit(1);
    }
    char is_sign[30] = {0};
    recv(sockfd, is_sign, sizeof(is_sign), 0);
    printf("%s\n", is_sign);

    while(1) {
        char buff[512] = {0};
        char buffer[1024] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        sprintf(buffer,"%s:%s", user.name, buff);
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("send()");
            exit(1);
        }
        signal(SIGINT, Stop);
        //recv(sockfd, buff, sizeof(buff), 0);
        //printf("Server : %s\n", buff);
    }
    return 0;
}

void Stop(int signo) {
    char have_logout[]
    send(sockfd, );
}
