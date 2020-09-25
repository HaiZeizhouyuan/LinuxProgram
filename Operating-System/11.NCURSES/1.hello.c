/*************************************************************************
	> File Name: 1.hello.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月22日 星期六 21时05分10秒
 ************************************************************************/

#include<ncurses.h>


int main() {
    initscr();
    printw("Hello World !!!");
    printw("Hi Hi Hi!!!");
    refresh();
    getchar();
    endwin();
    return 0;
	return 0;
}
