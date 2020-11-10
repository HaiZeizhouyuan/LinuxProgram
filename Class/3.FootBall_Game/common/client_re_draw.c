/*************************************************************************
	> File Name: client_re_draw.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月05日 星期四 15时04分37秒
 ************************************************************************/

#include "head.h"
extern struct BallStatus ball_status;
extern struct Score score;
extern struct User *red_team, *blue_team;
extern WINDOW *Football, *Football_t, *Score;
extern struct Bpoint ball;
extern int red_num, blue_num;
extern struct Map court;

void re_drew_ball() {
    wattron(Football, ball_status.by_team ? COLOR_PAIR(6) : COLOR_PAIR(2));
    w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
    wattroff(Football, ball_status.by_team ? COLOR_PAIR(6) : COLOR_PAIR(2));
}


void re_drew_players() {
    wattron(Football_t, COLOR_PAIR(2));
    for (int i = 0; i < red_num; i++) {
        w_gotoxy_putc(Football_t, red_team[i].loc.x, red_team[i].loc.y, 'k');
        w_gotoxy_puts(Football_t, red_team[i].loc.x + 1, red_team[i].loc.y - 1, red_team[i].name);
    }
    wattron(Football_t, COLOR_PAIR(6));
    for (int i = 0; i < blue_num; i++) {
        w_gotoxy_putc(Football_t, blue_team[i].loc.x, blue_team[i].loc.y, 'k');
        w_gotoxy_puts(Football_t, blue_team[i].loc.x, blue_team[i].loc.y - 1, blue_team[i].name);
    }
    wattroff(Football_t, COLOR_PAIR(6));

}

void re_drew_score(struct Score *score) {
    char tmp_red[5] = {0}, tmp_blue[5] = {0};
    sprintf(tmp_red, "%d", score->red);
    sprintf(tmp_blue, "%d", score->blue);
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

void client_re_draw() {
    werase(Football_t);
    werase(Score);
    box(Football_t, 0, 0);
    wattron(Football, COLOR_PAIR(2));
    box(Football, 0, 0);
    wattroff(Football, COLOR_PAIR(2));
    box(Score, 0, 0);
    court_draw();
    re_drew_ball();
    re_drew_players();
    re_drew_score(&score);
    wrefresh(Football_t);
    wrefresh(Football);
    wrefresh(Score);


}


