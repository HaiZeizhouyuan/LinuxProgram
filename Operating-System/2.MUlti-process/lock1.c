/*************************************************************************
	> File Name: lock1.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 20:53:34 2020
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
    sleep(10);
    flock(lock->_fileno, LOCK_UN);
    fclose(lock);
    return 0;
}
