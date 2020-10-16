/*************************************************************************
	> File Name: fread.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月17日 星期四 10时06分42秒
 ************************************************************************/

#include "head.h"
 
int main() {
    int fp;
    int nread;
    char buff[2024];
    fp = open("./fread.txt", O_RDONLY);
    while((nread = read(fp, buff, 1024)) > 0) {
        printf("%s\n", buff);
    }
    return 0;
}
