/*************************************************************************
	> File Name: const.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月15日 星期六 15时46分04秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main() {
    const int a = 10;
    const int *p = &a;
    int b = a;
    int *q = p;
    printf("a = %d, *p = %d\n", a, *p);
	return 0;
}
