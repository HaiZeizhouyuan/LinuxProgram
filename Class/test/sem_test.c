/*************************************************************************
	> File Name: test/sem_test.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月25日 星期五 15时38分51秒
 ************************************************************************/

#include "head.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
static int sem_id = 0;

static int set_semvaalue() {
    union semun sem_union;
    sem_union.val = 3;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        return 0;
    }
    return 1;
}

static void del_semvalue() {
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        fprintf(stderr, "Failed to delete semaphare\n");
    }
}

static int semaphore_pv(int op) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_pv falied!\n");
        return 0;
    }
    return 1;
}

void *func(void *arg) {
    int tmp = *(int *)arg;
    for (int i = 0; i < 3; i++) {
        if (!semaphore_pv(-1)) {
            exit(1);
        }
        time_t t;
        time(&t);
        printf(BLUE"P:%c at %s\n"NONE, tmp, ctime(&t));
        fflush(stdout);
        sleep(2);
        time(&t);
        printf(GREEN"V:%c at %s\n"NONE, tmp, ctime(&t));
        fflush(stdout);
        if (!semaphore_pv(1)) {
            exit(1);
        }
        sleep(2);
    }
    sleep(3);
    printf("\n%d - finished\n", getpid());
}

int main() {
    int i = 0;
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if (!set_semvaalue()) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    
    pthread_t tid[6];
    int data[10];
    for (int i = 0; i < 6; i++) {
        data[i] = i;
        pthread_create(&tid[i], NULL, func, (void *)&data[i]);
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(tid[i], NULL);
    }
    sleep(3);
    del_semvalue();
    return 0;
}
