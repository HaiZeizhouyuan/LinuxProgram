/*************************************************************************
	> File Name: client.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 19时46分33秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
struct User *blue_team;
struct User *red_team;
int red_sockfd, blue_sockfd;
int sockfd = -1, team = -1, server_port = 0;
char server_ip[20] = {0};
char name[20] = {0};
char *conf = "./football.json";
char chat_msg[512];
int main(int argc, char **argv) {
    int opt;
    struct LogRequest request;
    struct LogResponse response;
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));

    while ((opt = getopt(argc, argv, "h:p:t:m:n:")) != -1) {
        switch(opt) {
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'h':
                strcpy(server_ip, optarg);
                break;
            case 'n':
                strcpy(name, optarg);
                break;
            case 't':
                team = atoi(optarg);
                break;
            case 'm':
                strcpy(chat_msg, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-hptmn]!\n", argv[0]);
                exit(1);
        }
    }
    

    if (server_port == 0) server_port = atoi(get_cjson_value(conf, "SERVERPORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_cjson_value(conf, "SERVERIP"));
    if (!strlen(name)) strcpy(name, get_cjson_value(conf, "NAME"));
    if (team == - 1) team = atoi(get_cjson_value(conf, "TEAM"));
    if (strlen(chat_msg) == 0) strcpy(chat_msg, get_cjson_value(conf, "LOGMSG"));
    DBG(GREEN"Get server_port = %d, server_ip = %s, name = %s, team = %d, LOGMSG = %s  success!\n"NONE, server_port, server_ip, name, team, chat_msg);

    red_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));
    blue_team = (struct User *)calloc(MAX_TEAM, sizeof(struct User));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);
    socklen_t len = sizeof(server);
    if ((sockfd = socket_udp()) < 0){
        perror("socket_udp()");
        exit(1);
    }
    
    DBG(GREEN"client create success...\n"NONE);
    strcpy(request.msg, chat_msg);
    strcpy(request.name, name);
    request.team = team;
    sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&server, len);
    
    fd_set wfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    //设置超时时间为５秒，0毫秒
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);

    if (retval < 0) { //出错
        perror("select()");
        exit(1);
    } else if (retval) {
        int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0,(struct sockaddr *)&server, &len);
        if (ret != sizeof(response) || response.type) { //response.type == 1服务端是拒绝连接
            printf("The Game Server refused your logion.\n\tThis May be helpful:%s\n", response.msg);
            exit(1);
        }
    } else {//超时
        printf("The Game Server is out of service!.\n");
        exit(1);
    }
    printf("Server : %s\n", response.msg);
    connect(sockfd, (struct sockaddr *)&server, len);
    pthread_t recv_t;
    pthread_create(&recv_t, NULL, client_recv, NULL);
    struct FootBallMsg msg;
    strcpy(msg.name, name);
    msg.team = team;
    signal(SIGINT, client_exit);
    msg.type = FT_WALL;
    while(1) {
        bzero(&msg.msg, sizeof(msg.msg));
        scanf("%[^\n]s", msg.msg);
        getchar();
        if (strcmp(msg.msg, "#1") == 0) {
            msg.type = FT_MSG;
            continue;
        }
        int ret = send(sockfd, (void *)&msg, sizeof(msg), 0);
        DBG(BLUE"Send Msg have success!, ret = %d\n"NONE, ret);

    }
    
    return 0;
}
