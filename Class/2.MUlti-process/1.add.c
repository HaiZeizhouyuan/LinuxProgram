/*************************************************************************
	> File Name: 1.add.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  2 14:21:22 2020
 ************************************************************************/

#include "head.h"

char num_file[] = "./.num";
char lock_file[] = "./.lock";
struct Num {
    int now, sum;//now 要加的值, 
};

size_t set_num(struct Num *num) {
    FILE *f = fopen(num_file, "w");
    size_t nwrite = fwrite(num, sizeof(struct Num), 1, f);
    fclose(f);
    return nwrite;
}

size_t get_num(struct Num *num) {
    FILE *f = fopen(num_file, "r");
    if (f == NULL) {
        fclose(f);
        perror("fopen_r()");
        return -1;
    }
    size_t nread = fread(num, sizeof(struct Num), 1, f);
    if (nread < 0) {
        fclose(f);
        perror("fread()");
        return -1;
    }
    fclose(f);
    return nread;
}

void do_add(int max, int x) {
    struct Num num;
    while(1) {
        FILE *lock = fopen(lock_file, "w");
        flock(lock->_fileno, LOCK_EX);//等待, 加锁
        if (get_num(&num) < 0) {
            fclose(lock);
            continue;
        }
        DBG(GREEN"<Debug>"NONE" : <%d> now = %d, sum = %d\n", x, num.now, num.sum);
        if (num.now > max) {
            break;
        }
        num.sum += num.now;
        num.now ++;
        set_num(&num);
        flock(lock->_fileno, LOCK_UN);//解锁
        fclose(lock);
    }
}

//./a.out 10000 5
int main (int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage : %s max ins\n", argv[0]);
        exit(1);
    }

    int max = atoi(argv[1]);//atoi()把字符串转换成整形, max最大值
    int ins = atoi(argv[2]);//ins 是进程数
    struct Num num;
    pid_t pid;//进程类型的进程pid
    num.now = 0;
    num.sum = 0;
    int x = 0;
    set_num(&num);
    for (int i = 0; i < ins; i++) {
        if ((pid = fork()) < 0) {
            perror("fork()");
            exit(1);
        }
        x = i;
        if (pid == 0) break;
    }
    if (pid == 0) {
        do_add(max, ins);
        exit(0);
    }

    while (ins) {
        wait(NULL);
        ins--;
    }

    get_num(&num);
    printf("sum = %d\n", num.sum);
    return 0;
}

