/*************************************************************************
	> File Name: child_num.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 14:44:12 2020
 ************************************************************************/

#include "head.h"

#define max_n 100

int main () {
    pid_t pid;
    int x = 0;
    for (int i = 0; i < max_n; i++) {
        if ((pid = fork()) < 0) {
            perror("fork()");
            DBG(GREEN"<Dbug>"NONE" : %d child have problem\n", i);
            exit(1);
        }
        x = i;
        if (pid == 0) break; //子进程停止循环，跳出．父循环继续生．
    }
    if(pid == 0) {
        printf("I am %d child\n", x);
        DBG(GREEN"<Debug>"NONE" : I am %d child\n", x);
    } else {
        for (int i = 0; i < max_n; i++) {
            wait(NULL);
        }
    }
    return 0;
}

