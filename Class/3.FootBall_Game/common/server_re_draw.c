/*************************************************************************
	> File Name: server_re_draw.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月29日 星期四 17时38分52秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11

extern struct User *red_team,  *blue_team;
extern WINDOW *Football, *Football_t;
extern struct BallStatus ball_status;
extern struct Bpoint ball;
extern struct Map court;
extern struct Score score;

void re_drew_ball() {
    if (ball_status.v.x || ball_status.v.y) {
      //  show_message(NULL, NULL, "have change", 1);
        double flay_time = sqrt(pow(ball_status.v.x, 2) + pow(ball_status.v.y, 2)) / sqrt(pow(ball_status.a.x, 2) + pow(ball_status.a.y, 2));
        char tmp[512] = {0};
        sprintf(tmp, "time out = %lf, ax = %lf, ay = %lf\n", flay_time, ball_status.a.x, ball_status.a.y);
        double t = 0.10;
        if (t >= flay_time) {
            show_message(NULL, NULL, tmp, 1);
            bzero(&ball_status.v, sizeof(ball_status.v));
            bzero(&ball_status.a, sizeof(ball_status.a));
        } else {
            ball.x += (ball_status.v.x * t + (ball_status.a.x * pow(t, 2)) / 2);
            ball_status.v.x += ball_status.a.x * t;
            ball.y += (ball_status.v.x * t + (ball_status.a.x * pow(t, 2)) / 2);
            ball_status.v.y += ball_status.a.y * t;
            if ((ball.x <= 0) && ( ball.y > (court.height / 2 - 3)) && (ball.y < court.height / 2 + 2)) {
                score.red += 1;
                ball.x = 2;
                ball.y = court.height / 2;
                struct FootBallMsg msg;
                msg.type = FT_WALL;
                sprintf(msg.msg, "%s of %s team, get 1 score", ball_status.name, ball_status.by_team ? "blue" : "red");
                send_all(&msg);
            } else if ((ball.x >= court.width) && (ball.y > (court.height / 2 - 3)) && (ball.y < (court.height / 2 + 2))) {
                score.blue += 1;
                ball.x = court.width - 3;
                ball.y = court.height / 2;
                struct FootBallMsg msg;
                msg.type = FT_WALL;
                sprintf(msg.msg, "%s of %s team , get 1 score", ball_status.name, ball_status.by_team ? "blue" : "red");
            }
            if (ball.y >= court.height) ball.y = court.height;
            if (ball.y <= 0) ball.y = 0;
            bzero(&ball_status.v, sizeof(ball_status.v));
            bzero(&ball_status.a, sizeof(ball_status.a));
        }
    }
   // show_message(NULL, NULL, "re_draw finish", 1);
    w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
    if (ball_status.by_team) {
        wattron(Football, COLOR_PAIR(6));
    } else {
        wattron(Football, COLOR_PAIR(2));
    }
   // w_gotoxy_putc(Football, (int)ball.x + 1, (int)ball.y, ACS_DEGREE);//???
    wattron(Football, COLOR_PAIR(3));
}

void re_draw_player(int team, char *name, struct Point *loc) {
    if (team) {
        wattron(Football, COLOR_PAIR(6));
    } else {
        wattron(Football, COLOR_PAIR(2));
    }
    w_gotoxy_putc(Football, loc->x, loc->y, 'k');
    w_gotoxy_puts(Football, loc->x , loc->y - 1, name);
}

void re_drew_team(struct User *team) {
   // show_message(NULL, NULL, "re_drew_team", 1);
    for (int i = 0; i < MAX_TEAM; i++) {
        if (team[i].online == 0) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }   
}


void re_drew() {
    werase(Football_t);
    box(Football, 0, 0);
    box(Football_t, 0, 0);
    re_drew_team(red_team);
    re_drew_team(blue_team);
    re_drew_ball();
    wrefresh(Football);
    wrefresh(Football_t);

}
