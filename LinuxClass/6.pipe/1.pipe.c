/*************************************************************************
	> File Name: 1.pipe.c
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 11:47:35 2020
 ************************************************************************/

#include<stdio.h>
#include "head.h"

char buff[20];

int main (int argc, char **argv) {
    pid_t pid;
    int pipefd[2];
    pipe(pipefd);
    memset(buff, 0, sizeof(buff));
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }

    if (pid ==  0) {
        close(pipefd[0]);
        while(1) {
            scanf("%s", buff);
            write(pipefd[1], buff, strlen(buff));
            memset(buff, 0, sizeof(buff));
        }
    } else {
        close(pipefd[1]);
        while(1) {
            read(pipefd[0], buff, sizeof(buff));
            printf("%s\n", buff);
            memset(buff, 0, sizeof(buff));
        }
    }
    return 0;
}
