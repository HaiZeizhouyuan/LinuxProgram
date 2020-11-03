/*************************************************************************
	> File Name: ../common/send_chat.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月14日 星期日 16时51分32秒
 ************************************************************************/

#include "head.h"

extern int sockfd;
extern WINDOW *Write;
extern struct FootBallMsg chat_msg;

void send_chat() {
    echo();
    nocbreak();
    wclear(Write);
    box(Write, 0, 0);
    wrefresh(Write);
    bzero(chat_msg.msg, sizeof(chat_msg.msg));
    w_gotoxy_puts(Write, 1, 1, "Input Message : ");
    mvwscanw(Write, 2, 1, "%[^\n]s", chat_msg.msg);
    if (strlen(chat_msg.msg))
        send(sockfd, (void *)&chat_msg, sizeof(chat_msg), 0);
    wclear(Write);
    box(Write, 0, 0);
    wrefresh(Write);
    noecho();
    cbreak();
}
