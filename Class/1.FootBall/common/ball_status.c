/*************************************************************************
	> File Name: ball_status.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月03日 星期五 02时20分25秒
 ************************************************************************/

#include "head.h"

extern WINDOW *Message, *Write;
extern struct Bpoint ball;
extern struct BallStatus ball_status;

int can_kick(struct Point *loc, int strength) {
    int px = loc->x, py = loc->y;
    if (abs(px - (int)ball.x) <= 2 && abs(py - (int)ball.y) <= 2) {
        if (px == (int)ball.x && py == (int)ball.y) {
            return 0;
        }
        double dx = (ball.x - px) / sqrt(((ball.x - px), 2) + pow((ball.y - py), 2));
        double dy = (ball.y - py) / sqrt(((ball.x - px), 2) + pow((ball.y - py), 2));
        ball_status.a.x = -3 * dx;
        ball_status.a.y = -3 * dy;
        ball_status.v.x = strength * 0.2 * 40 * dx;
        ball_status.v.y = strength * 0.2 * 40 * dy;
        char arr[512] = {0};
        sprintf(arr, "Point(%d, %d), Bpoint(%.3lf, %.3lf), v(%.3lf, %.3lf), a(%.3lf, %.3lf)", px, py, ball.x, ball.y, ball_status.v.x, ball_status.v.y, ball_status.a.x, ball_status.a.y);
        show_message(NULL, NULL, arr, 1);
        return 1;
    }
    return 0;
}
