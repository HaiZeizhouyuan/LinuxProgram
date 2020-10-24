/*************************************************************************
	> File Name: main_shell.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月20日 星期二 09时59分16秒
 ************************************************************************/

#include "head.h"
 
int main() {
    FILE *f = NULL;
    if ((f = popen("bash ../System/Script/Disk.sh >> shell.txt", "w")) == NULL) {
        perror("fopen()");
        exit(1);
    }
    return 0;
}
