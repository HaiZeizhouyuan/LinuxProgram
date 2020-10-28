/*************************************************************************
	> File Name: test.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月27日 星期二 14时59分22秒
 ************************************************************************/

#include<stdio.h>
#include <curses.h>

int main () {
    initscr();
    //box(stdscr, ACS_VLINE, ACS_HLINE);
    newwin(5, 4, 1, 1);
    box(stdscr, '|', '-');
    //move(LINES/2, COLS/2);
    //waddstr(stdscr, "Hello world!");
    refresh();
    getch();
    endwin();
    return 0;
}
