/*************************************************************************
	> File Name: client1.c
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 09:35:30 2020
 ************************************************************************/

#include "chat.h"

struct Message *share_memory = NULL;
struct Message tmp;
int main (int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "n:")) != -1) {
        switch(opt) {
            case 'n':
                strcpy(tmp.name, optarg);
                break;
            default:
                fprintf(stderr, " Usage : %s -n name!\n",argv[0]);
                break;
        }
    }

    int shmid;
    key_t key = ftok(".", 2015);
    if ((shmid = shmget(key, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget()");
        exit(1);
    }
    if ((share_memory = shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat");
        exit(1);
    }

    if (share_memory->pid <= 0) {
        fprintf(stderr, "Please run server first!\n");
        exit(1);
    }

    while (1) {
        scanf("%[^\n]s", tmp.msg);
        getchar();
        if (!strlen(tmp.msg)) continue;
        while (1) {
            if (!strlen(share_memory->msg)) {
                pthread_mutex_lock(&share_memory->mutex);
                break;
            }
        }
        strcpy(share_memory->name, tmp.name);
        strcpy(share_memory->msg, tmp.msg);
        pthread_mutex_unlock(&share_memory->mutex);
        pthread_cond_signal(&share_memory->cond);//当条件达成时,发送信号.
        memset(tmp.msg, 0, sizeof(tmp.msg));
        DBG(GREEN"<Dbug>"NONE " : After siganl : %s\n", share_memory->msg);
    }
}


