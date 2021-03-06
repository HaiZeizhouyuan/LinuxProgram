/*************************************************************************
	> File Name: game_over.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2021年01月18日 星期一 15时59分53秒
 ************************************************************************/

#include "head.h"
extern struct Score score;
extern struct User *red_team, *blue_team;
extern WINDOW *Football, *Football_t, *Message, *Message_t,  *Help, *Score, *Write;
extern int team;
void game_over() {
    destroy_win(Football_t);
    destroy_win(Message_t);
    destroy_win(Help);
    destroy_win(Score);
    destroy_win(Write);
    refresh();
    

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
        
    WINDOW *game_over_w;
    int w = COLS - 4, h = LINES - 4;
    game_over_w = create_newwin(w, h, 1,  1);
    box(game_over_w, 0, 0);
    wrefresh(game_over_w);
    if (score.red > score.blue) {
        if (team == 0) w_gotoxy_puts(game_over_w,  w / 2 - strlen("恭喜你们队赢得比赛") / 2 , h / 2, "恭喜你们队赢得比赛");
        else  w_gotoxy_puts(game_over_w,  w / 2 - strlen("不要气馁，再来一局") / 2, h / 2, "不要气馁，　再来一局");
    } else {
        if (team == 1) w_gotoxy_puts(game_over_w,  w / 2 - strlen("恭喜你们队赢得比赛") / 2, h / 2, "恭喜你们队赢得比赛");
        else  w_gotoxy_puts(game_over_w,  w / 2 - strlen("不要气馁，再来一局") / 2, h / 2, "不要气馁，　再来一局");     
    }
    char temp[50];
    sprintf(temp, "红队共进了%d个球, 蓝队共进%d个球", score.red, score.blue);
    w_gotoxy_puts(game_over_w, w / 2 - strlen(temp) / 2, h / 2 + 2, temp);
    wrefresh(game_over_w);
    while(1)
    return ;
}
