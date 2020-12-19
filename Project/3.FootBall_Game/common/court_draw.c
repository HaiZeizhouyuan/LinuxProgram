/*************************************************************************
	> File Name: court_draw.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月10日 星期二 10时29分32秒
 ************************************************************************/

#include "head.h"

extern WINDOW *Football, *Football_t;
extern struct Map court;
void court_draw () {
    mvwprintw(Football_t, 0, (court.width + 4) / 2, "多人足球小游戏");
    wrefresh(Football_t); 
    wattron(Football, COLOR_PAIR(3));
    for (int i = 1; i < court.height - 1; i++) {
        w_gotoxy_puts(Football,court.width / 2, i, "|");    
    }

    w_gotoxy_puts(Football, court.width / 2 + 1, court.height / 2 + 2, ".");
    w_gotoxy_puts(Football, court.width / 2 + 3, court.height / 2 + 1, ".");
    w_gotoxy_puts(Football, court.width / 2 + 4, court.height / 2, ".");
 
    w_gotoxy_puts(Football, court.width / 2 + 1, court.height / 2 - 2, ".");
    w_gotoxy_puts(Football, court.width / 2 + 3, court.height / 2 - 1, ".");
    w_gotoxy_puts(Football, court.width / 2 + 4, court.height / 2, ".");
      
    w_gotoxy_puts(Football, court.width / 2 - 1, court.height / 2 - 2, ".");
    w_gotoxy_puts(Football, court.width / 2 - 3, court.height / 2 - 1, ".");
    w_gotoxy_puts(Football, court.width / 2 - 4, court.height / 2, ".");

    w_gotoxy_puts(Football, court.width / 2 - 1, court.height / 2 + 2, ".");
    w_gotoxy_puts(Football, court.width / 2 - 3, court.height / 2 + 1, ".");
    w_gotoxy_puts(Football, court.width / 2 - 4, court.height / 2, ".");
    
    //球门
    for (int i = 1; i < 6; i++) {
        wattron(Football, COLOR_PAIR(2));
        w_gotoxy_puts(Football, i, court.height / 2 - 3, "-");
        w_gotoxy_puts(Football, i, court.height / 2 + 3, "-");
        wattron(Football, COLOR_PAIR(6));
        w_gotoxy_puts(Football, court.width - 1 - i, court.height / 2 - 3, "-");
        w_gotoxy_puts(Football, court.width - 1 - i, court.height / 2 + 3, "-");
    }
    for (int i = 1; i < 3; i++) {
        wattron(Football, COLOR_PAIR(2));
        w_gotoxy_puts(Football, i, court.height / 2 - 1, "-");
        w_gotoxy_puts(Football, i, court.height / 2 + 1, "-");
        wattron(Football, COLOR_PAIR(6));
        w_gotoxy_puts(Football, court.width - 1 - i, court.height / 2 - 1, "-");
        w_gotoxy_puts(Football, court.width - 1 - i, court.height / 2 + 1, "-");
    }

    for (int i = 0 ; i < 3; i++) {
        wattron(Football, COLOR_PAIR(2));
        w_gotoxy_puts(Football, 6, court.height / 2 - i, "|");
        w_gotoxy_puts(Football, 6, court.height / 2 + i, "|");
        wattron(Football, COLOR_PAIR(6));
        w_gotoxy_puts(Football, court.width - 7, court.height / 2 - i, "|");
        w_gotoxy_puts(Football, court.width - 7, court.height / 2 + i, "|");
    }
    wattron(Football, COLOR_PAIR(2));
    w_gotoxy_puts(Football, 3, court.height / 2, "|");
    wattron(Football, COLOR_PAIR(6));
    w_gotoxy_puts(Football, court.width - 4, court.height / 2, "|");
    return ;
}
