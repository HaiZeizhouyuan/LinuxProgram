/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 16:23:05 2020
 ************************************************************************/

#include "head.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s", argv[0]);
        exit(1);
    }

    int scokfd, port;
    char ip[20] = {0};
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);

    if ((sockfd = sockfd_connect(ip, port) )< 0){
        perror("sockfd_connet");
        exit(1);
    }

    while(1) {
        char buff[512] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        if (send(sockfd, buff, strlen(buff), 0 ) < 0) {
            perror("send()");
            exit(1);
        }

    }
    return 0;
}
