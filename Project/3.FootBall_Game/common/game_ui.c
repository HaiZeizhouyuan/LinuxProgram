/*************************************************************************
	> File Name: game_ui.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月26日 星期一 20时59分43秒
 ************************************************************************/

#include "head.h"

struct Map court;//球场大小
extern WINDOW *Football, *Football_t, *Message, *Message_t,  *Help, *Score, *Write;//窗体
extern struct Bpoint ball;//球的位置
extern struct BallStatus ball_status;
extern int msg_num;
extern struct Score score;

WINDOW *create_newwin(int width, int height, int startx, int starty) {
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}
void destroy_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void gotoxy(int x, int y) {
    move(y, x);
}

void gotoxy_putc(int x, int y, char c) {
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
}
void gotoxy_puts(int x, int y, char* s) {
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, char c) {
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
}
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s) {
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
}


void show_help() {
    //wattron(Help, COLOR_PAIR(2));
    w_gotoxy_puts(Help, 7, 2, "HELP");
    //wattron(Help, COLOR_PAIR(3));
    w_gotoxy_puts(Help, 3, 4, "W/A/S/D - Move");
    w_gotoxy_puts(Help, 3, 5, "J - Stop ball");
    w_gotoxy_puts(Help, 3, 6, "K - Kick ball");
    w_gotoxy_puts(Help, 3, 7, "L - Carry ball");

}

void show_score() {
    char tmp_red[5] = {0}, tmp_blue[5] = {0};
    sprintf(tmp_red, "%d", score.red);
    sprintf(tmp_blue, "%d", score.blue);
    w_gotoxy_puts(Score, 7, 2, "SCORE");
    wattron(Score, COLOR_PAIR(2));
    w_gotoxy_puts(Score, 5, 3, "RED");
    wattroff(Score, COLOR_PAIR(2));
    w_gotoxy_puts(Score, 9, 3, ":");
    wattron(Score, COLOR_PAIR(6));
    w_gotoxy_puts(Score, 11, 3, "BLUE");
    wattron(Score, COLOR_PAIR(2));
    w_gotoxy_puts(Score, 6, 4, tmp_red);
    wattroff(Score, COLOR_PAIR(2));
    w_gotoxy_putc(Score, 9, 4, ':');
    wattron(Score, COLOR_PAIR(6));
    w_gotoxy_puts(Score, 12, 4, tmp_blue);
    wattroff(Score, COLOR_PAIR(6));
}

void initfootball() {
    initscr();//初始化屏幕
    clear();//清空屏幕
    curs_set(FALSE);
    if (!has_colors() || start_color() == ERR) {
        endwin();
        fprintf(stderr, "终端不支持颜色!\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);//绿色
    init_pair(2, COLOR_RED, COLOR_BLACK);//红色
    init_pair(3, COLOR_WHITE, COLOR_BLACK);//白色
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);//黄色
    init_pair(5, COLOR_CYAN, COLOR_BLACK);//青色
    init_pair(6, COLOR_BLUE, COLOR_BLACK);//蓝色
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);//洋红
    init_pair(8, COLOR_BLACK, COLOR_GREEN);
    init_pair(9, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(10, COLOR_BLACK, COLOR_RED);
    init_pair(11, COLOR_BLACK, COLOR_BLUE);
    init_pair(12, COLOR_BLACK, COLOR_YELLOW);
    

    Football_t = create_newwin(court.width + 4, court.height + 2, court.start.x - 2, court.start.y - 1);
    Football = subwin(Football_t, court.height, court.width, court.start.y, court.start.x);
    WINDOW *Message_t = create_newwin(court.width + 4, 7,  court.start.x - 2, court.start.y + court.height + 1);
    Message = subwin(Message_t, 5, court.width + 2, court.start.y + court.height + 2 , court.start.x - 1);
    scrollok(Message, 1);
    Help = create_newwin(20, court.height + 2,  court.start.x + court.width + 2, court.start.y - 1);
    Score = create_newwin(20, 7,  court.start.x + court.width + 2 , court.start.y + court.height + 1);
    Write = create_newwin(court.width + 4 + 20, 5,  court.start.x - 2, court.start.y + 1 + court.height + 7);
    wattron(Football, COLOR_PAIR(2));
    box(Football, 0, 0);
    box(Football_t, 0, 0);
    box(Message_t, 0, 0);
    box(Help, 0, 0);
    box(Score, 0, 0);
    box(Write, 0, 0);
    court_draw();
    wattron(Football, COLOR_PAIR(3));//设置足球端口的颜色
    w_gotoxy_putc(Football, ball.x, ball.y, 'o');//设置足球的位置
    wattroff(Football, COLOR_PAIR(3));  
    
    show_help();
    show_score();
}


void show_message(WINDOW *win,struct User *user, char *msg, int type) {
    if (win == NULL) win = Message;
    time_t time_now = time(NULL);
    struct tm *tm  = localtime(&time_now);
    char timestr[20] = {0};
    char msgname[30] = {0};
    wattron(win, COLOR_PAIR(6));
    sprintf(timestr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
    w_gotoxy_puts(win, 1, msg_num, timestr);
    if(type == 1) {
        wattron(win, COLOR_PAIR(4));
        strcpy(msgname, "Sys Info : ");
    } else if (type == 2) {
        wattron(win, user->team ? COLOR_PAIR(6) : COLOR_PAIR(2));
        sprintf(msgname, "%s : ", user->name);
    } else {
        if (user->team) {
            wattron(win, COLOR_PAIR(6));
        } else {
            wattron(win, COLOR_PAIR(2));  
        }
        sprintf(msgname,"[全部]%s :", user->name );
    } 
    if (msg_num <= 4) {
        w_gotoxy_puts(win, 10, msg_num, msgname);
        wattron(win, COLOR_PAIR(3));
        w_gotoxy_puts(win, 10 + strlen(msgname), msg_num, msg);
        wattron(win, COLOR_PAIR(5));
        w_gotoxy_puts(win, 1, msg_num, timestr);
        msg_num++;
            
    } else {
        msg_num = 4;
        scroll(win);
        w_gotoxy_puts(win, 10, msg_num, msgname);
        wattron(win, COLOR_PAIR(3));
        w_gotoxy_puts(win, 10 + strlen(msgname), msg_num, msg);
        wattron(win, COLOR_PAIR(5));
        w_gotoxy_puts(win, 1, msg_num, timestr);
        msg_num++;
            
    }
    wrefresh(win);

}

