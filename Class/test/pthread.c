/*************************************************************************
	> File Name: pthread.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月19日 星期一 21时20分33秒
 ************************************************************************/

#include "head.h"
int x;
void *print(void *arg) {
    int tmp = *(int *)arg;
    //    DBG(GREEN"<Debug>"NONE " : I am  %d  child\n", *(int *)arg);
    printf("i am %d child\n", tmp);
    //
}

int main (int argc, char **argv) {
    int tmp_arg[100] = {0};
    pthread_t tid[10];
    for (int i = 1; i <= 6; i++) {
        tmp_arg[i] = i;
        pthread_create(&tid[i], NULL, print, (void *)&tmp_arg[i]);
        //sleep(0);
        //    
    }
    for (int i = 1; i <= 6; i++) {
        pthread_join(tid[i], NULL);
            
    }
        //sleep(4);
    //    return 0;
    //
}

