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
    char str[20] = {"hello "}, str2[20] = {"world"};
    strcat(str, str2);
    printf("%s\n", str);
    return 0;
}
