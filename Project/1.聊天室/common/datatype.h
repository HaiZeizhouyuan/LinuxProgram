/*************************************************************************
	> File Name: datatype.h
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:48:13 2020
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#define CHAT_SYN 0x01
#define CHAT_FIN 0x02
#define CHAT_ACK 0x04
#define CHAT_HEART 0x08
#define CHAT_PRI 0x10
#define CHAT_PUB 0x20
#define CHAT_FUNC 0x40
#define CHAT_SYS 0x80
#define CHAT_NAK 0x100
#define CHAT_FIN_1 0x200
#define RECV_READY 0x2100
#define RECV_END 0x3000
#define SEND_FILE_TO 0x800
#define SEND_FILE_ALL 0x1000
#define SEND_FAIL 0x1021
#define FUNC_CHECK_ONLINE 0x01
#define FUNC_CHANGE_NAME 0x02

struct User{
    char name[20];
    char chat_name[20];
    //char real_name[20];
    int flag;//心跳次数
    char ip[20];
    char id[20];
    int fd;
    int online;
    int issend;
};

struct FileMsg {
    char name[512];
    long size;
    char buff[4096];
    char recv_name[20];
    int type;
};

struct ChatMsg {
    int type;
    int opt;
    char name[20];
    char msg[2000];
};


#endif
