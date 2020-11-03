/*************************************************************************
	> File Name: show_strength.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年07月02日 星期四 14时00分24秒
 ************************************************************************/

#include "head.h"

extern WINDOW *Write;
extern int sockfd;

void show_strength() {
    int maxx, maxy, strength;
    getmaxyx(Write, maxy, maxx);
    int unit = maxx / 5;
    for (int i = 2; i < maxx - 2; i++) {
        if (i < unit || i > 4 * unit ) {
            wattron(Write, COLOR_PAIR(8));
        } else if (( i < unit * 2) || i > unit * 3) {
            wattron(Write, COLOR_PAIR(9));
        } else {
            wattron(Write, COLOR_PAIR(10));
        }
        mvwaddch(Write, 2, i, ' ');
        wrefresh(Write);
    }
    wattron(Write, COLOR_PAIR(3));
    int tmp[5] = {1, 2, 3, 2, 1};
    int offset = 1;
    int tmp_x = 2;
    setnonblocking(0);
    
    while (1) {
        char c = getchar();
        if (c != -1) {
            if (c == ' ' || c == 'k') {
                mvwaddch(Write, 1, tmp_x, ' ');
                mvwaddch(Write, 3, tmp_x, ' ');
                setblocking(0);
                break;
            }
        }
        usleep(10000);
        mvwaddch(Write, 1, tmp_x, ' ');
        mvwaddch(Write, 3, tmp_x, ' ');
        tmp_x += offset;
        mvwaddch(Write, 1, tmp_x, '|');
        mvwaddch(Write, 3, tmp_x, '|');
        mvwaddch(Write, 4, maxx, ' ');
        if (tmp_x >= maxx - 2) offset = -1;
        else if (tmp_x  <= 2) offset = 1;
        wrefresh(Write);
    }
    struct FootBallMsg msg;
    bzero(&msg, sizeof(msg));
    char info[512] = {0};
    sprintf(info, "strength = %d", tmp[tmp_x / unit]);
    show_message(NULL, NULL, info, 1);
    msg.type = FT_CTL;
    msg.ctl.action = ACTION_KICK;
    msg.ctl.strength = tmp[tmp_x / unit];
    send(sockfd, (void *)&msg, sizeof(msg), 0);

    return ;
}

