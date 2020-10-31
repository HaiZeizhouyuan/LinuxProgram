/*************************************************************************
	> File Name: chat_ui.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月23日 星期日 14时10分30秒
 ************************************************************************/

#include "head.h"
extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
extern int msg_num;
WINDOW *create_newwin(int width, int height, int startx, int starty) {
    WINDOW *win;
    win = newwin(height, width,  starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void gotoxy(int x, int y) {
    move(y, x);//横轴增加y，纵轴增加x
}

void gotoxy_putc(int x, int y, char c) {
    move(y, x);
    addch(c);//将单个字符添加到窗口,然后移动光标, 自动换行.和滚动
    refresh();
}

void gotoxy_puts(int x, int y, char *s) {
    move(y, x);
    addstr(s);//将字符串添加到窗口，并移动光标
    refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, char c) {
    mvwaddch(win, y, x, c);//将光标移动到x, y输出字符
    wrefresh(win);
}

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s) {
    mvwprintw(win, y, x, s);
    wrefresh(win);
}

void show_help() {
    wattron(sub_help_win, COLOR_PAIR(2));
    w_gotoxy_puts(sub_help_win, 2, 2, "Help:");
    wattron(sub_help_win, COLOR_PAIR(7));
    w_gotoxy_puts(sub_help_win, 3, 3, "使用＠user进行私聊");
    w_gotoxy_puts(sub_help_win, 3, 4, "使用#1查询在线人数");
    w_gotoxy_puts(sub_help_win, 3, 5, "使用#2匿名");
}

void init_ui (){
    initscr();
    clear();
    if(!has_colors() || start_color() == ERR) {
        endwin();
        fprintf(stderr, "终端不支持彩色!\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);//CYAN青色
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

    init_pair(8, COLOR_BLACK, COLOR_GREEN);
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(11, COLOR_BLACK, COLOR_BLUE);
    init_pair(12, COLOR_BLACK, COLOR_YELLOW);

    msg_win = create_newwin(MSG_WIDTH, MSG_HEIGHT, 2, 1);
    sub_msg_win = subwin(msg_win, MSG_HEIGHT - 2, MSG_WIDTH - 2, 2, 3);//输出消息子窗口
    help_win = create_newwin(HELP_WIDTH, MSG_HEIGHT, MSG_WIDTH + 2, 1);
    sub_help_win = subwin(help_win, MSG_HEIGHT - 2, HELP_WIDTH - 2, 2, MSG_WIDTH + 3);
    input_win = create_newwin(HELP_WIDTH + MSG_WIDTH, INPUT_HEIGHT, 2, MSG_HEIGHT + 1);
    sub_input_win = subwin(input_win, INPUT_HEIGHT - 2, MSG_WIDTH + HELP_WIDTH - 2, MSG_HEIGHT + 1, 3);


    show_help();
    scrollok(sub_msg_win, 1);
    scrollok(sub_input_win, 1);
    box(msg_win, 0, 0);
    box(sub_msg_win, 0, 0);
    box(input_win, 0, 0);
    box(sub_input_win, 0, 0);
    box(help_win, 0, 0);
    box(sub_help_win, 0, 0);
 
    wrefresh(help_win);
    wrefresh(msg_win);
    wrefresh(input_win);
    refresh();
    sleep(5);
}

void print_message(WINDOW *win, struct ChatMsg *msg, int type) {
    time_t time_now = time(NULL);
    struct tm *tm  = localtime(&time_now);
    char timestr[20] = {0};
    wattron(win, COLOR_PAIR(2));
    sprintf(timestr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
    if (msg_num >= MSG_HEIGHT - 2) {
        msg_num = MSG_HEIGHT - 3;
        scroll(win);
    }
    w_gotoxy_puts(win, 1, msg_num, timestr);
    wattron(win, COLOR_PAIR(3));
    if(type || msg->type & CHAT_SYS) {
        strcpy(msg->name, "Sys Info");
        wattron(win, COLOR_PAIR(7));
    } else {
        wattron(win, COLOR_PAIR(6));
    }
    w_gotoxy_puts(win, 10, msg_num, msg->name);
    wattron(win, COLOR_PAIR(3));
    w_gotoxy_puts(win, 10 + strlen(msg->name) + 1, msg_num, msg->msg);
    //int len = strlen(msg->msg) + strlen(msg->name) + 11;
    //msg_num += ceil(len * 1.0 / MSG_WIDTH);
    msg_num++;
    sleep(1);
}
