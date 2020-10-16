/*************************************************************************
	> File Name: lock2.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  2 10:01:16 2020
 ************************************************************************/

#include "head.h"

char lock_file[] = "./.lock";

int main () {
    FILE *lock = fopen(lock_file, "w");
    if (lock == NULL) {
        perror("fopen");
        exit(1);
    }
    flock(lock->_fileno, LOCK_EX);
    printf("Locked!\n");
    flock(lock->_fileno, LOCK_UN);
    fclose(lock);
    return 0;
}
