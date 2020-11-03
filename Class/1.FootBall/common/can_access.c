/*************************************************************************
	> File Name: can_access.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月03日 星期二 19时45分27秒
 ************************************************************************/

#include "head.h"
extern struct Bpoint ball;
int can_access(struct Point *loc) {
    if (sqrt(pow(loc->x - ball.x, 2) + pow(loc->y - ball.y, 2)) <= 2) return 1;
    return 0;
}

