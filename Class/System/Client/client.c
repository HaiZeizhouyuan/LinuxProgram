/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年10月09日 星期五 18时27分33秒
 ************************************************************************/

#include "head.h"

char buff[100];

void run_client(int signum) {
    printf("I'm Client!\n");
    return ;
}

void run_master(int signum) {
    printf("I'm Master!\n");
    return ;
}

int main () {
    pid_t pid;
    int shmid;
    key_t key = ftok(".", 2020);
    if (shmid = (shmget(key, sizeof(buff), IPC_CREAT | 0666)) < 0) {
        perror("shmget()");
        exit(1);
    }

    if ((share_memory = (buff)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat()");
        exit(1);
    }
    memset(share_memory, 0, sizeof(buff)); 
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
   

    int flag;
    if (pid == 0) {
        while(1){
            scanf("%d", &flag);
            if (flag == 1) {
                kill(getppid(), SIGUSER1);
            } else {
                kill(getppid(), SIGUSER2);
            }
        }
    } else {
        signal(SIGUSER1, run_client);
        signal(SIGUSER2, run_master);
    }
    return 0;
}


