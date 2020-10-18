/*************************************************************************
	> File Name: 1.Healthv1.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月16日 星期五 18时25分13秒
 ************************************************************************/

#include "../common/head.h"
#include "./health.h"



struct Share *share_memory = NULL;
cJSON *conf = NULL;
void doing_process(int who) {
    char msg[512] = {0};
    char config_p[30];
    char desc[20];
    int port;
    strcpy(msg, who ? "Slave" : "Master");
    sprintf(config_p, "%s%s", msg, "Port");
    port = get_json_valueint(conf, msg, config_p);
    strcpy(desc, get_json_valuestring(conf, msg, "Desc"));
    printf("I'm %s\n", msg);
    printf("port = %d, desc = %s\n", port, desc);
    
}

int main() {
    int who = 0, shmid;
    DBG(GREEN"Start ..\n"NONE);
    key_t key = ftok(".", 209);
    if ((shmid = shmget(key, sizeof(struct Share), IPC_CREAT | 0666)) < 0) {
        perror("shmget()");
        exit(1);
    }
    DBG(GREEN"shmget success ....\n"NONE);
    if ((share_memory = (struct Share *)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat()");
        exit(1);
    }

    DBG(GREEN"shmat success...\n"NONE);

    memset(share_memory, 0, sizeof(struct Share));

    DBG(GREEN"Share Mem memset success...\n"NONE);
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&share_memory->mutex, &attr);

    DBG(GREEN"Init mutex...\n"NONE);

    pthread_condattr_t c_attr;
    pthread_condattr_init(&c_attr);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&share_memory->cond, &c_attr);
    
    DBG(GREEN"Init condition...\n"NONE);
    share_memory->type = 0;
    DBG(GREEN"Program set to be Master default...\n"NONE);
    DBG(GREEN"Forking...\n"NONE);

    DBG(GREEN"Checking config file...\n"NONE);
    char buff[512] = {0};
    conf = get_cjson("./conf.json");
    if (conf == NULL) {
        perror("get_string()");
        exit(1);
    }
    
    
    pid_t pid;
    int x = 0;
    for (int i = 0; i < 2; i++) {
        if ((pid = fork()) < 0 ) {
            perror("fork()");
            exit(1);
        }
        x = i;
        if (pid == 0) break;
    }

    if (pid == 0 && x == 0) {
        int pnum = 0;
        DBG(RED"The %dth Process act as a Controler!\n"NONE, pnum);
        while(1) {
            int cmd;
            DBG(YELLOW"please Input You Cmd(0 for Master, 1 for Salve):\n"NONE);
            scanf("%d", &cmd);
            if (cmd == 0) {
                share_memory->type = 0;
            } else {
                share_memory->type = 1;
            }
            pthread_cond_signal(&share_memory->cond);
        }
    } else if (pid == 0 && x == 1){
        int pnum = 1;
        DBG(PINK"The %dth Process act as a Master!\n"NONE, pnum);//0输出Master
        while(1) {
            if (share_memory->type == 1) {
                pthread_mutex_lock(&share_memory->mutex);
                DBG(BLUE"Master Process Waiting for Signal..\n"NONE);
                while (share_memory->type == 1) {
                    pthread_cond_wait(&share_memory->cond, &share_memory->mutex);
                }
                pthread_mutex_unlock(&share_memory->mutex);
                DBG(RED"The %dth end sleep, start Master\n"NONE, pnum);
                while(1) {
                    if (share_memory->type == 1) break;
                    doing_process(0);
                    sleep(2);
                }
            } else {
                DBG(RED"The %dth Process Start while Master\n"NONE, pnum);
                while(1) {
                    if (share_memory->type == 1) {
                        DBG(RED"exit master, start slave!\n"NONE);
                        break;
                    }
                    doing_process(0);
                    sleep(2);
                }
            }
        }
    } else {
        int pnum = 2;
        DBG(BLUE"The %dth Process act as a Slave!\n"NONE, pnum);
        while(1) {
            if (share_memory->type == 0 ) {
                pthread_mutex_lock(&share_memory->mutex);
                DBG(BLUE"Slave Process Waiting for Signal..\n"NONE);
                while(share_memory->type == 0) {
                    pthread_cond_wait(&share_memory->cond, &share_memory->mutex);//等待信号，　等到时加锁．
                }
                //此时type == 1, 执行Master;
                pthread_mutex_unlock(&share_memory->mutex);
                DBG(RED"The %dth end sleep, start Slave\n"NONE, pnum);
                while(1) {
                    if (share_memory->type == 0) break;
                    doing_process(2);
                    sleep(2);
                }
            } else {
                DBG(RED"THe %dth Process Start while Slave!\n"NONE, pnum);
                while(1) {
                    if (share_memory->type == 0) {
                        DBG(RED"exit slave, Start Master\n "NONE);
                        break;
                    }
                    doing_process(2);
                    sleep(2);
                }
            }
        }
    }
    return 0;
}
