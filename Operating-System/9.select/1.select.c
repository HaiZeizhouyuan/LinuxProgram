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
    
    FD_ZERO(&rfds);//清空文件描述符集　void FD_ZERO(fd_set *set);
    FD_SET(0, &rfds);//把标准输入文件的异常信息注册到文件描述符集中
    
    /* Wait up to five seconds.*/
    //设置等待时间，等待５秒
    
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
        while ((re = read(0, buffer, strlen(buffer))) > 0) {
            printf("%s", buffer);
        }
        printf("\n");
    /* FD_ISSET(0, &rfds) will be true. */   
    } else {
        printf("No data within five seconds.\n");
    }
    exit(EXIT_SUCCESS);
    return 0;
}

