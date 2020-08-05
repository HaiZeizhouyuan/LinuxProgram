/*************************************************************************
	> File Name: 1.shm_add.c
	> Author: 
	> Mail: 
	> Created Time: Mon Aug  3 17:06:44 2020
 ************************************************************************/

#include "head.h"

struct Num {
    int now, sum;
    pthread_mutex_t mutex;
};

struct Num *share_memory = NULL;

void do_add(int max, int x) {
    while(1) {
        pthread_mutex_lock(&share_memory->mutex);
        DBG(GREEN"<Debug>"NONE" : <%d> now = %d, sum = %d\n", x, share_memory->now, share_memory->sum);
        if (share_memory->now > max) {
            pthread_mutex_unlock(&share_memory->mutex);
            break;
        }

        share_memory->sum += share_memory->now;
        share_memory->now++;
        pthread_mutex_unlock(&share_memory->mutex);
        usleep(20);
    }
}


int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s max ins\n", argv[0]);
        exit(1);
    }
    int max = atoi(argv[1]);
    int ins = atoi(argv[2]);

    pid_t pid;
    int shmid;
    key_t key = ftok(".", 2009);
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
    int x = 0;
    for (int i = 0; i < ins; i++) {
        if ((pid = fork()) < 0) {
            perror("fork()");
            exit(1);
        }

        x = i;
        if (pid == 0) break;
    }
    if (pid == 0) {
        do_add(max, x);
        exit(0);
    }

    while (ins) {
        wait(NULL);
        ins--;
    } 

    printf("sum = %d\n", share_memory->sum);
}


