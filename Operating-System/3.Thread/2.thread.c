/*************************************************************************
	> File Name: 2.thread.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 18:43:46 2020
 ************************************************************************/

#include "head.h"

struct Arg {
    char name[20];
    int age;
} Arg;

struct Arg arg_in;

void *print(void *arg) {
        struct Arg tmp = *(struct Arg *)arg;
        printf("helle HaiZei %s, he is %d years old!\n", tmp.name, tmp.age);
    }

int main (){
    pthread_t tid;
    arg_in. age = 18;
    strcpy(arg_in.name, "zhouyuan");
    pthread_create(&tid, NULL,print, (void *)&arg_in);
    sleep(1);
    return 0;
}

