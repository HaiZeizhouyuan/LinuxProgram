/*************************************************************************
	> File Name: duojincheng.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri Jul 31 16:49:57 2020
 ************************************************************************/

#include "head.h"

int main () {
    pid_t pid, pid_w;
    
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        //printf("child Process spid = %d\n", getpid());
        printf("In child Process!\n");
        sleep(1);
        //execlp("ls", "ls","/etc",".", NULL);
    } else {
        pid_w = wait(NULL);
        printf("In parent Process!\n");
    }

    exit(0);
}
