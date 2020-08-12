/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  8 14:31:10 2020
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

struct User {
    int fd;
    char name[20];
    int online;
    char real_name[20];
    char chat_name[20];
    char ip[20];
    char id[50];
};

char *get_conf(const char *conf, const char *key);
int make_non_block(int fd);
int make_block(int fd);
extern char conf_value_ans[512];
int socket_create(int port);//创建端口port
int socket_connect(char *ip, int port);//客户端连接
void get_info(char *pathname, struct User *user, const char *ip);
#endif
