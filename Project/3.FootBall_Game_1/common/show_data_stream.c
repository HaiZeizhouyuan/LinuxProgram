/*************************************************************************
	> File Name: show_data_stream.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月01日 星期日 11时43分03秒
 ************************************************************************/

#include "head.h"

extern char data_stream[20];
extern WINDOW *Help;
extern struct Map court;
extern int help_num;

void show_data_stream(int type) {
    for (int i = 18; i >= 1; i --) {
        data_stream[i] = data_stream[i - 1];
    }
    data_stream[1] = type;
    for (int i = 1; i < 19; i++) {
        switch(data_stream[i]) {
            case 'l':
                wattron(Help, COLOR_PAIR(8));//log
                break;
            case 'c':
                wattron(Help, COLOR_PAIR(9));//carry
                break;
            case 'k':
                wattron(Help, COLOR_PAIR(10));//kick
                break;
            case 's':
                wattron(Help, COLOR_PAIR(11));//stop
                break;
            case 'n':
                wattron(Help, COLOR_PAIR(12));//normal
                break;
            case 'e':
                wattron(Help, COLOR_PAIR(7));//end
                break;

        }
        mvwprintw(Help, court.height - 1, i, " ");
        wattron(Help, COLOR_PAIR(3));
    }
    wrefresh(Help);

}

