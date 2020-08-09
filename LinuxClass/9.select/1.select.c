/*************************************************************************
	> File Name: 1.select.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 09:00:15 2020
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int main(void) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    
    /* Wait up to five seconds. */
    
    tv.tv_sec = 5;//秒
    tv.tv_usec = 0;//微秒
    retval = select(1, &rfds, NULL, NULL, &tv);
    
    /* Don't rely on the value of tv now! */
    
    if (retval == -1) {
        perror("select()");
    } else if (retval){
        printf("Data is available now.\n");
        char buffer[1024];
        ssize_t re, we;
        re = read(0, buffer, strlen(buffer));
        we = write(0, buffer, strlen(buffer));
    /* FD_ISSET(0, &rfds) will be true. */   
    } else {
        printf("No data within five seconds.\n");
    }
    exit(EXIT_SUCCESS);
    return 0;
}

