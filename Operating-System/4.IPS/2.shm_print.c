/*************************************************************************
	> File Name: 2.shm_add.c
	> Author: 
	> Mail: 
	> Created Time: Mon Aug  3 17:06:44 2020
 ************************************************************************/

#include "head.h"
#define MAXSIZE 1024
struct Num {
    char buff[MAXSIZE];
    pthread_mutex_t mutex;
};

struct Num *share_memory = NULL;

void do_printf() {
    while(1) {
        if (strlen(share_memory->buff)) {
            pthread_mutex_lock(&share_memory->mutex);
            DBG(GREEN"<Debug>"NONE" : locked in Parent\n")
            printf(RED"<Parent>"NONE" : %s\n",share_memory->buff);
            memset(share_memory->buff, 0, sizeof(share_memory->buff));
            pthread_mutex_unlock(&share_memory->mutex);
        }
    }
}


int main (int argc, char **argv) {
    pid_t pid;
    int shmid;
    key_t key = ftok(".", 2109);
    if (shmid = shmget(key, sizeof(struct Num), IPC_CREAT | 0666) < 0) {
        perror("shmget()");
        exit(1);
    }

    if ((share_memory = (struct Num *)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat()");
        exit(1);
    }

    memset(share_memory, 0, sizeof(struct Num));
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);//PTHREAD_PROCESS_SHREAD可共享，锁是进程的
    pthread_mutex_init(&share_memory->mutex, &attr);
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }

    if (pid == 0) {
        while(1) {
            if (strlen(share_memory->buff)) continue;
                pthread_mutex_lock(&share_memory->mutex);
                DBG(GREEN"<Debug>"NONE " : locked in child\n");
                printf("child input:");
                scanf("%[^\n]s", share_memory->buff);
                getchar();
                DBG(PINK"<Child>"NONE" : %s\n", share_memory->buff);
                pthread_mutex_unlock(&share_memory->mutex);
                //usleep(20);
        }
    } else {
        do_printf();
        wait(NULL);
    }
   
}


