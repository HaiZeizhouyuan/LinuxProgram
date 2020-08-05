/*************************************************************************
	> File Name: cp.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Jul 26 16:27:20 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define MAX_BUFF 1024
#include "head.h"
//./a.out file1 file2
int main (int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, " Usage : %s src dest\n", argv[0]);
        exit(1);
    }
    int fd_in, fd_out;

    if ((fd_in = open(argv[1], O_RDONLY)) == -1) {
        perror("open()");
        exit(1);
    }
;
    if ((fd_out = creat(argv[2], 0666)) == -1) {
        perror("creat()");
        exit(1);
    }

    ssize_t nread, nwrite;
    char buff[MAX_BUFF + 5] = {0};
    while ((nread = read(fd_in, buff, MAX_BUFF) > 0)) {
        DBG(GREEN"<Debug> "NONE" nread = %ld\n strlen(buff) = %ld", nread, strlen(buff));
        if ((nwrite = write(fd_out, buff, strlen(buff))) != nread){
            perror("write()");
            exit(1);
        }
        memset(buff, 0, sizeof(buff));
    }
#ifdef _D
    printf("定义了_D\n");
#else
    printf("未定义_D\n");
#endif 
    close (fd_in);
    close(fd_out);
    return 0;
}


