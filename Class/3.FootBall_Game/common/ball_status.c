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
    int ux =loc->x - 2, uy = loc->y - 1;
    sprintf(tmp, "loc_x = %d, lox_y = %d, ball_x = %f, ball_y = %f", ux, uy, ball.x, ball.y);
    show_message(NULL, NULL, tmp, 1);
    if (abs(ux - (int)ball.x) <= 2 && abs(uy - (int)ball.y) <= 2) {
        if (ux == (int)ball.x && uy == (int)ball.y) return 0;
        double v_tmp = (500.0 * strength) * 0.2;//初始速度
        sprintf(tmp, "v_tmp = %lf", v_tmp);
        show_message(NULL, NULL, tmp, 1);
        
		double tx = ball.x - ux;
		double ty = ball.y - uy;
		double dx = tx / sqrt(pow(tx, 2) + pow(ty, 2));
		double dy = ty / sqrt(pow(tx, 2) + pow(ty, 2));
		ball_status.a.x = -3.0 * dx;
		ball_status.a.y = -3.0 * dy;
		ball_status.v.x = v_tmp * dx;
		ball_status.v.y = v_tmp * dy;
		return 1;
    }
    return 0;
}
