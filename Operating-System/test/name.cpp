/*************************************************************************
	> File Name: name.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  2 12:00:19 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define num 3

int main () {
    srand(time(0));
    int val = rand() % num;
    switch(val) {
        case 0:
            printf("z\n");
            break;
        case 1:
            printf("y\n");
            break;
        case 2:
            printf("t\n");
            break;
    }
    return 0;
}
