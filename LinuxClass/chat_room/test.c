/*************************************************************************
	> File Name: test.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月16日 星期日 18时25分24秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#define true 1
#define FIN 0x01
int main () {
    int b = 0x04;
    b |= FIN;
    printf("%d\n", b);

    return 0;
}
