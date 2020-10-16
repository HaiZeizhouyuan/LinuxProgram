/*************************************************************************
	> File Name: msg.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月23日 星期三 20时28分34秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;//消息的类型
    char mtext[80];//消息的内容
};

static void usage(char *prog_name, char *msg) {
    if (msg != NULL)  fputs(msg, stderr);
    fprintf(stderr, "Usage: %s [options]\n", prog_name);
    fprintf(stderr, "Options are:\n");
    fprintf(stderr, "-s        send message using msgsnd()\n");
    fprintf(stderr, "-r        read message using msgrcv()\n");
    fprintf(stderr, "-t        message type (default is 1)\n");
    fprintf(stderr, "-k        message queue key (default is 1234)\n");
    exit(EXIT_FAILURE);
}

static void send_msg(int qid, int msgtype, char *s_msg) {
    struct msgbuf msg;
    time_t t;
    msg.mtype = msgtype;
    time(&t);
    snprintf(msg.mtext, sizeof(msg.mtext), "At %s : %s", ctime(&t), s_msg);
    if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
    printf("sent: %s\n", msg.mtext);
}

static void get_msg(int qid, int msgtype) {
    struct msgbuf msg;
    if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), msgtype, MSG_NOERROR | IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("No message available for msgrcv()\n");
    } else {
        printf("message received: %s\n", msg.mtext);
    }
}

int main(int argc, char *argv[]) {
    int qid, opt;
    int mode = 0;               /* 1 = send, 2 = receive */
    int msgtype = 1;
    int msgkey = 1234;
    char msg[1024] = {0};
    while ((opt = getopt(argc, argv, "srt:k:m:")) != -1) {
        switch (opt) {
            case 's'://发送端
                mode = 1;
                break;
            case 'r'://接收端
                mode = 2;
                break;
            case 't':
                msgtype = atoi(optarg);
                if (msgtype <= 0)
                    usage(argv[0], "-t option must be greater than 0\n");
                break;
            case 'k':
                msgkey = atoi(optarg);
                break;
            case 'm':
                strcpy(msg, optarg);
                break;
            default:
                usage(argv[0], "Unrecognized option\n");
            
        }
    }
    if (mode == 0)  usage(argv[0], "must use either -s or -r option\n");
    qid = msgget(msgkey, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    if (mode == 2)
        get_msg(qid, msgtype);
    else
        send_msg(qid, msgtype, msg);
    exit(EXIT_SUCCESS);
}

