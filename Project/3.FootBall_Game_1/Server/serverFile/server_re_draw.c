/*************************************************************************
	> File Name: server_re_draw.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月29日 星期四 17时38分52秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11

extern struct User *red_team,  *blue_team;
extern WINDOW *Football, *Football_t, *Score;
extern struct BallStatus ball_status;
extern struct Bpoint ball;
extern struct Map court;
extern struct Score score;

void re_drew_ball() {
    if (ball_status.by_team) {
        wattron(Football, COLOR_PAIR(6));
    } else {
        wattron(Football, COLOR_PAIR(2));
    }

    if (ball_status.v.x || ball_status.v.y) {
        double flay_time = 0.1 * sqrt(pow(ball_status.v.x, 2) + pow(ball_status.v.y, 2)) / sqrt(pow(ball_status.a.x, 2) + pow(ball_status.a.y, 2));
        double t = 0.10;
        while(t < flay_time) {
            w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, ' ');
            ball.x += (ball_status.v.x * t + (ball_status.a.x * pow(t, 2)) / 2);
            ball_status.v.x += ball_status.a.x * t;
            ball.y += (ball_status.v.y * t + (ball_status.a.y * pow(t, 2)) / 2);
            ball_status.v.y += ball_status.a.y * t;
            if ((ball.x <= 6) && ( ball.y > (court.height / 2 - 3)) && (ball.y < court.height / 2 + 3)) {
                bzero(&ball_status.v, sizeof(ball_status.v));
                bzero(&ball_status.a, sizeof(ball_status.a));
                map_change();
                court_draw();
                w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
                w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, ' ');
                score.blue += 1;
                sleep(1);
                srand(time(0));
                double rd = rand() % (court.width / 8);
                ball.x = court.width / 2 + rd;
                ball.y = court.height / 2;
                
                struct FootBallMsg msg;
                msg.type = FT_WALL;
                sprintf(msg.msg, "%s of %s team, get 1 score", ball_status.name, ball_status.by_team ? "blue" : "red");
                send_all(&msg);
                t = flay_time;
            } else if ((ball.x >= court.width - 7) && (ball.y > (court.height / 2 - 3)) && (ball.y < (court.height / 2 + 3))) {
                bzero(&ball_status.v, sizeof(ball_status.v));
                bzero(&ball_status.a, sizeof(ball_status.a));
                map_change();
                court_draw();
                w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
                w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, ' ');
                score.red += 1;
                sleep(1);
                srand(time(0));
                double rd = rand() % (court.width / 8);
                
                ball.x = court.width / 2 + rd;
                ball.y = court.height / 2;
                struct FootBallMsg msg;
                msg.type = FT_WALL;
                sprintf(msg.msg, "%s of %s team , get 1 score", ball_status.name, ball_status.by_team ? "blue" : "red");
                t = flay_time;
            }
            if  (ball.y >= court.height - 1) {
                ball.y = court.height - 1;
                t = flay_time;
            }
            if (ball.y <= 0) {
                ball.y = 0;
                t = flay_time;
            }
            t += 0.10;
            court_draw();
            w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
            map_change();
            usleep(100000);
        }
        char tmp[512] = {0};
        sprintf(tmp, "time out = %lf, ax = %lf, ay = %lf\n", flay_time, ball_status.a.x, ball_status.a.y);
        bzero(&ball_status.v, sizeof(ball_status.v));
        bzero(&ball_status.a, sizeof(ball_status.a));
        show_message(NULL, NULL, tmp, 1);
    }
    court_draw();
    w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
}

void re_draw_player(int team, char *name, struct Point *loc) {
    if (team) {
        wattron(Football_t, COLOR_PAIR(6));
    } else {
        wattron(Football_t, COLOR_PAIR(2));
    }
    w_gotoxy_putc(Football_t, loc->x, loc->y, 'k');
    w_gotoxy_puts(Football_t, loc->x + 1 , loc->y - 1, name);
    wattroff(Football_t, team ? COLOR_PAIR(6) : COLOR_PAIR(2));
}

void re_drew_team(struct User *team) {
   // show_message(NULL, NULL, "re_drew_team", 1);
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online == 0) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }   
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

void re_drew() {
    if (score.red == 5 || score.blue == 5) {
        sleep(11);
        exit(0);
    }
    werase(Football_t);
    werase(Score);
    box(Football_t, 0, 0);
    wattron(Football, COLOR_PAIR(2));
    box(Football, 0, 0);
    wattroff(Football, COLOR_PAIR(2));
    box(Score, 0, 0);
    re_drew_ball();
    re_drew_team(red_team);
    re_drew_team(blue_team);

    re_drew_score(&score);
    wrefresh(Football_t);
    wrefresh(Football);
    wrefresh(Score);

}
