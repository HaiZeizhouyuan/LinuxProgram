/*************************************************************************
	> File Name: 0.one_thread.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Wed 12 Aug 2020 02:37:21 PM CST
 ************************************************************************/

#include "head.h"
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port!\n", argv[0]);
        exit(1);
    }

    int server_listen,sockfd, port;
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        exit(1);
    }
    
    while (1) {
        struct sockaddr_in client;
        struct User user;
        char buff[512] = {0};
        memset(&user, 0, sizeof(user));
        socklen_t len = sizeof(client);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
            perror("accept()");
            exit(1);
        }
        get_info("../../x.测评记录/names", &user, inet_ntoa(client.sin_addr));
        DBG("Ip = %s, name = %s", inet_ntoa(client.sin_addr), user.real_name);
        read(sockfd, buff, sizeof(buff));
        printf(BLUE"%s"NONE" : %s\n", user.real_name, buff);
        close(sockfd);
    }

    return 0;
}
