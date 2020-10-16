/*************************************************************************
	> File Name: sem.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月25日 星期五 09时42分59秒
 ************************************************************************/

#include "head.h"

int sem_id, um_num, stu_num;
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buff;
};

static int init_semval(int val) {
    union semun sem_set;
    sem_set.val = val;
    if (semctl(sem_id, 0, SETVAL, sem_set) == - 1) {
        perror("semctl");
        return 0;
    }
    return 1;
}

static int del_semval() {
    union semun sem_del;
    if (semctl(sem_id, 0, IPC_RMID, sem_del) == -1) {
        perror("sem_del");
        return 0;
    }
    return 1;
}

static int sem_pv(int op) {
    //p : -1, v +1;
    struct sembuf sem_pv;
    sem_pv.sem_num = 0;
    sem_pv.sem_op = op;
    sem_pv.sem_flg = SEM_UNDO;//程序结束时(不论正常或不正常)，保证信号值会被重设为semop()调用前的值。
    if (semop(sem_id, &sem_pv, 1) == - 1) {//第三个参数是操作的数量
        perror("semop");
        return 0;
    }
    return 1;
}

int P() {
    return sem_pv(-1);
}
int V() {
    return sem_pv(1);
}
void *func(void *arg) {
    int tmp = *(int *)arg;
    time_t t;
    int i = 3;
    while(i--) {
        int time_out = rand() % 10;
        sleep(time_out);
        int j = 0;
        while(um_num == 0) sleep(++j);
        if (!P()) {
            perror("sem_p");
        }
        time(&t);
        printf(GREEN"student %d out at %s"RED"umbrella have %d\n\n"NONE, tmp, ctime(&t), --um_num);
        fflush(stdout);
        int time_on = rand() % 10;
        sleep(time_on);
        if (!V()) {
            perror("sem_v");
        }
        time(&t);
        printf(BLUE"student %d have back at %s"RED"umbrella have %d\n\n"NONE, tmp, ctime(&t), ++um_num);
    }
    sleep(1);
    printf("stdeunt %d have finish!\n", tmp);
}



int main(int argc, char **argv) {
    int opt;
    int flag_s = 0, flag_u = 0;
    while((opt = getopt(argc, argv, "s:u:")) != -1) {
        switch(opt) {
            case 's' :
                flag_s = 1;
                stu_num = atoi(optarg);
                break;
            case 'u' :
                flag_u = 1;
                um_num = atoi(optarg);
                break;
            default :
                fprintf(stderr, "Usage: %s -s -u!\n", argv[0]);
        }
    }
    if (!flag_s || !flag_u || argc < 5) {
        fprintf(stderr, "Usage : %s stu_num um_num!\n", argv[0]);
        exit(1);
    }

    printf(YELLOW"It started raining outside! room have %d students have %d umbrella!\n\n"NONE, stu_num, um_num);
    key_t mykey;
    mykey = ftok("key", 1000);
    if ((sem_id = semget(mykey, 1, IPC_CREAT | 0666)) == -1) {
        perror("semget failed");
        exit(1);
    }
    if(!init_semval(um_num)) {
        perror("init");
        return 1;
    }
    pthread_t tid[stu_num + 5];
    int stu[10];
    for (int i = 0; i < stu_num; i++) {
        stu[i] = i;
        pthread_create(&tid[i], NULL, func, (void *)&stu[i]);
    }
    for (int i = 0; i < stu_num; i++) {
        pthread_join(tid[i], NULL);
    }
    sleep(3);
    printf(YELLOW"Rain have stop!room have student %d, have umbrella %d\n"NONE, stu_num, um_num);
    del_semval();
    return 0;
}
