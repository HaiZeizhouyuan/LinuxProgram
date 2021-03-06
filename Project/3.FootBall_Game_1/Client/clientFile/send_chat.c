/*************************************************************************
	> File Name: send_chat.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月30日 星期五 15时05分12秒
 ************************************************************************/

#include "head.h"
 
extern int sockfd;
extern WINDOW *Write;
extern struct FootBallMsg chat_msg;

void send_chat() {
    DBG(GREEN"Start scanf!\n"NONE);
    echo();
    nocbreak();
    curs_set(true);
    bzero(chat_msg.msg, sizeof(chat_msg.msg));
    w_gotoxy_puts(Write, 1, 1, "Input Message : ");
    mvwscanw(Write, 2, 1, "%[^\n]s", chat_msg.msg);
    if (strlen(chat_msg.msg)) {
        send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
    }
    wclear(Write);
    box(Write, 0, 0);
    wrefresh(Write);
    noecho();
    cbreak();
}


