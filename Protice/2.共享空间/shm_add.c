/*************************************************************************
	> File Name: 1.shm_create.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 15时33分13秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int max;

struct Num {
    int now, sum;
    pthread_mutex_t mutex;
};

struct Num *share_memory = NULL;

void do_add(int x) {
    while(1) {
        pthread_mutex_lock(&share_memory->mutex);
        if (share_memory->now > max) {
            pthread_mutex_unlock(&share_memory->mutex);
            break;
        }
        share_memory->sum += share_memory->now;
        share_memory->now += 1;
        pthread_mutex_unlock(&share_memory->mutex);
        printf("<%d> now : %d, sum : %d\n", x,  share_memory->now, share_memory->sum);
        usleep(20);
    }
}


//./a.out max ins : max 要加的数的最大值, ins是线程数
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s max ins!\n", argv[0]);
    }
    max = atoi(argv[1]);
    int ins = atoi(argv[2]);

    int shmid;
    key_t key = ftok(".", 2020);
    if ((shmid = shmget(key, sizeof(struct NUm *), IPC_CREAT | 0666)) < 0) {
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
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&share_memory->mutex, &attr);
    int x;
    pid_t pid;
    for (int i = 0; i < ins; i++) {
        if((pid = fork())< 0) {
            perror("fork()");
            exit(1);
        }
        x = i;
        if (pid == 0) break;
    }
    if (pid == 0) {
        do_add(x);
        exit(0);
    }

    while(ins) {
        wait(NULL);
        ins--;
    }

    printf("sum = %d\n", share_memory->sum);
    return 0;
}
