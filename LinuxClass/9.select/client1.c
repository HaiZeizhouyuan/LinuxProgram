/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 16:23:05 2020
 ************************************************************************/

#include "work_list.h"
struct User user;
int h_flag = 0,p_flag = 0,  n_flag = 0; 
//./client -h 47.94.34.228 -p 8888 -n zhouyuan
int main (int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "h:p:n:")) != -1) {
        switch(opt) {
            case 'h':
                h_flag = 1;
                strcpy(user.ip, optarg);
                break;
            case 'p':
                p_flag = 1;
                strcpy(opt, optarg);
                break;
            case 'n':
                flag = 1;
                strcpy(user.real_name, optarg);
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

    int sockfd, port;
    char ip[20] = {0};
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);

    if ((sockfd = socket_connect(ip, port)) < 0){//建立客户端
        perror("sockfd_connet");
        exit(1);
    }

    while(1) {
        char buff[512] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        if (send(sockfd, buff, strlen(buff), 0) < 0) {
            perror("send()");
            exit(1);
        }
        recv(sockfd, buff, sizeof(buff), 0);
        printf("Server : %s\n", buff);
    }
    return 0;
}
