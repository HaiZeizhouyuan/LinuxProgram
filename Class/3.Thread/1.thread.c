/*************************************************************************
	> File Name: 1.thread.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 18:19:50 2020
 ************************************************************************/

#include "head.h"

void *print(void *arg) {
    //int tmp = *(int *)arg;
    DBG(GREEN"<Debug>"NONE " : I am  %d  child\n", *(int *)arg);
    //printf("i am %d child\n", tmp);
}

int main (int argc, char **argv) {
    int tmp_arg[100] = {0};
    pthread_t tid[100];
    for (int i = 1; i <= 100; i++) {
        tmp_arg[i] = i;
        pthread_create(&tid[i], NULL, print, (void *)&tmp_arg[i]);
        //sleep(0);
    }
    for (int i = 1; i <= 100; i++) {
        pthread_join(tid[i], NULL);
    }
    //sleep(4);
    return 0;
}
