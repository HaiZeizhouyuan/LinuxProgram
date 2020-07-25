/*************************************************************************
	> File Name: ls1.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Jul 25 21:30:58 2020
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv) {
    int opt, l_flag = 0, a_flag = 0;
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch(opt) {
            case 'l':
                l_flag = 1;
                break;
            case 'a':
                a_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage : %s [-la]!\n", argv[0]);
                exit(1);
        }
    }
    if (l_flag == 1) {
        printf("has l\n");
    } else {
        printf("no l\n");
    }
    if (a_flag == 1) {
        printf("has a\n");
    } else {
        printf("no a\n");
    }

}
