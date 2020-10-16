/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2020年10月13日 星期二 11时48分39秒
 ************************************************************************/

#include "head.h"
#define MAXSIZE 1024
struct Num {
    char buff[MAXSIZE];
    pthread_mutex_t mutex;
};

struct Num *share_memory = NULL;

void *do_print(int signum) {
    printf(RED"<Parent>"NONE" : %s\n", share_memory->buff);
    memset(share_memory->buff, 0 ,sizeof(share_memory->buff));
    return NULL;
}


int main (int argc, const char **argv) {
    pid_t pid;
    int shmid;
    key_t key = ftok(".", 2020);
    if (shmid = (shmget(key, sizeof(struct Num), IPC_CREAT | 0666)) < 0) {
        perror("shmget()");
        exit(1);
    }

    if ((share_memory = (struct Num *)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat()");
        exit(1);
    }
    memset(share_memory, 0, sizeof(struct Num));
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) {
        while(1) {
            printf("Please Input :");
            scanf("%[^\n]s", share_memory->buff);
            DBG(PINK"<Child>"NONE" : %s\n", share_memory->buff);
            getchar();
            kill(getppid(), SIGUSR1);
        }
    } else {
        signal(SIGUSR1, do_print);
        wait(NULL);
    }
    return 0;
}

