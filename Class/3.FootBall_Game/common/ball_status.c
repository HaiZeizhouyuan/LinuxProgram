/*************************************************************************
	> File Name: ball_status.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月31日 星期六 19时09分48秒
 ************************************************************************/

#include "head.h"
#define PI (arccos(-1))
extern struct Bpoint ball;
extern struct BallStatus ball_status;

int can_kick(struct Point *loc, int strength) {///?????
    char tmp[512] = {0};
    sprintf(tmp, "loc_x = %d, lox_y = %d, ball_x = %f, ball_y = %f", loc->x, loc->y, ball.x, ball.y);
    show_message(NULL, NULL, tmp, 1);
    double dis = sqrt(pow(loc->x - ball.x, 2) + pow(loc->y - ball.y, 2));
    if (dis > 0 && dis <= 2) {
        double v_tmp = (40.0 * strength) * 0.2;
        sprintf(tmp, "v_tmp = %lf", v_tmp);
        show_message(NULL, NULL, tmp, 1);
    
        double angle = atan(fabs(loc->y - ball.y) / fabs(loc->x - ball.x));
        if (ball.x > loc->x) {
            ball_status.v.x = cos(angle) * v_tmp;
            ball_status.a.x = cos(angle) * 3;
        } else {
            ball_status.v.x = -cos(angle) * v_tmp;
            ball_status.a.x = -cos(angle) * 3;
        } 
        

        if (ball.y > loc->y) {
            ball_status.v.y = cos(angle) * v_tmp;
            ball_status.a.y = cos(angle) * 3;
        } else {
            ball_status.v.y = -cos(angle) * v_tmp;
            ball_status.a.y = -cos(angle) * 3;
        } 
        return 1;
    }
    return 0;
}
