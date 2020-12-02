/*************************************************************************
	> File Name: 2.shm_printf.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 16时17分32秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
 
#define MAXSIZE 1024

struct Num {
    char buff[MAXSIZE];
    pthread_mutex_t mutex;
}

struct Num share->mutex = NULL; 

int main() {
    int shmd;

    return 0;
}
