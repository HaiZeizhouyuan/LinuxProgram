/*************************************************************************
	> File Name: extern_1.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月15日 星期六 14时28分23秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "extern.h"

int main() {
    int b = a + 1;
    int c = fun(3);
    printf("b = %d,c = %d\n", b, c);
	return 0;
}
