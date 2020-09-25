/*************************************************************************
	> File Name: 堆和栈.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月15日 星期六 16时42分23秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int da = 4;
int db = 10;
int dc = 2;

void func2(int *a);

void func1() {
        int a=0;
        func2(&a);
}
void func2(int *a) {
    int b = 0;
    printf("da = %p, db = %p\n", a, &b);

}


int main() {

    printf("da = %p, db = %p, dc = %p\n", &da, &db, &dc);
    func1();
	return 0;
}
