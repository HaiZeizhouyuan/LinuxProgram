/*************************************************************************
	> File Name: 1.union.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月16日 星期三 18时25分15秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
 
union Data {
    int a;
    char b;
};

int main() {
    union Data data;
    data.a = 1;
    //printf("%d %d %d %d", data.b[0], data.b[1], data.b[2], data.b[3]);
    if (data.b == 1) {
        printf("小端!\n");
    } else {
        printf("大端!\n");
    }

    int c = 1;
   // printf("%d", *(char *)&c);
    if (*(char *)&c == 1) {
        printf("小端!\n");
    } else {
        printf("大端!\n");
    }
    return 0;
}
