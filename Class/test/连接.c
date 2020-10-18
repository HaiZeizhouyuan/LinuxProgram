/*************************************************************************
	> File Name: 连接.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月18日 星期日 20时31分09秒
 ************************************************************************/

#include<stdio.h>
 
void test(char *name) {
    printf("%s\n", name);
}

int main() {
    char str[20] = {"Sever"};
    char name[20];
    test(name);

    return 0;
}
