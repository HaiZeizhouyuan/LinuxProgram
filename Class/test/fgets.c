/*************************************************************************
	> File Name: fread.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月17日 星期四 10时06分42秒
 ************************************************************************/

#include "head.h"
 
int main() {
    FILE *fp;
    int nread;
    char buff[5000];
    fp = fopen("./fread.txt", "r");
    fgets(buff, 5000, fp);
    printf("%s\n", buff);
    return 0;
}
