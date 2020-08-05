/*************************************************************************
	> File Name: multi_process.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  1 09:54:17 2020
 ************************************************************************/

#include "head.h"

int m_flag = 0;

void open_vim();
int main (int argc, char **argv) {
    int opt;
    char msg[1024] = {0};
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                m_flag = 1;
                strcpy(msg, optarg);
                break;
            default :
                fprintf(stderr, "Usage : %s -m msg", argv[0]);
                break;
        }
    }

    DBG(GREEN"<Debug>"NONE " : m = %d\n", m_flag);

    //argc -= optind;
    //argv += optind;
    if (m_flag == 1) {
        printf("%s\n", msg);
        return 0;
    }

    open_vim();
    return 0;
}

void open_vim() {
    pid_t pid, pid_w;
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    } 
    
    if (pid == 0) {
        execlp("vim", "vim", "./tmp.txt", NULL);
    } else {
        wait(NULL);
        execlp("cat", "cat", "./tmp.txt", NULL);
    }
}
