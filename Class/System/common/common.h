/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  8 14:31:10 2020
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

char *get_conf(const char *conf, const char *key);
int make_non_block(int fd);
int make_block(int fd);
extern char conf_value_ans[512];
int socket_create(int port);//创建端口port
int socket_connect(char *ip, int port);//客户端连接
int socket_connect_timeout(char *host, int port, long timeout);
#endif
