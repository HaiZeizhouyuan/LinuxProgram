/*************************************************************************
	> File Name: 1.Healthv1.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月19日 星期一 15时13分31秒
 ************************************************************************/

#include "../common/head.h"
#include "./health.h"

struct Share *share_memory = NULL;

cJSON *conf = NULL, *slave = NULL;
int sockfd, master_listen;
int MasterPort, SlavePort;
char MasterDesc[20], SlaveDesc[20];
char ip[10] = {"8888"}, buff[1024];
size_t recv_size;
void doing_process(int who) {
    char msg[512] = {0};
    strcpy(msg, who ? "Slave" : "Master");
    if (!who) {
        printf("I'm %s, MasterPort = %d, Desc = %s\n", msg, MasterPort, MasterDesc);
        DBG(GREEN"Start read SysInfo!\n"NONE);
        DBG(GREEN"Get SlavePort and SlaveDesc success...\n"NONE);
        if ((sockfd = socket_connect(ip, SlavePort )) < 0) {
        perror("socket_connect");
        exit(1);
    }
    
    DBG(BLUE"socket connect success...\n"NONE);
    
    if ((master_listen = socket_create(MasterPort)) < 0) {
        perror("sock_create");
        exit(1);
    }

    DBG(BLUE"socket create success...\n"NONE);
        //while((recv_size = recv(sockfd, (void *)&buff, sizeof(buff), 0)) > 0) {
          //  printf("%s\n", buff);
        //}
    } else {
        printf("I'm %s, SlavePort = %d, Desc = %s\n", msg, SlavePort, SlaveDesc);
        pid_t pid;
        pthread_t tid[6];
        int num[10] = {0};
        for (int i = 0; i < 6; i++) {
            num[i] = i;
            pthread_create(&tid[i], NULL, get_info, (void *)&num[i]);
        }
        for (int i = 0; i < 6; i++) {
            pthread_join(tid[i], NULL);
        }
    }
    return ;
}

int main() {
    int who = 0, shmid;
    DBG(GREEN"Start..\n"NONE);
    key_t key = ftok(".", 2020);
    if ((shmid = shmget(key, sizeof(struct Share), IPC_CREAT | 0666)) < 0) {
        perror("shmget()");
        exit(1);
    }
    DBG(GREEN"Shmget success...\n"NONE);
    if ((share_memory = (struct Share *)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat()");
        exit(1);
    }
    DBG(GREEN"Shmat success...\n"NONE);
    memset(share_memory, 0, sizeof(struct Share));
    
    DBG(GREEN"Share Mem memset success ...\n"NONE);

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
    DBG(GREEN"Program set to be Master default..\n"NONE);
    DBG(GREEN"Forking...\n"NONE);

    if ((conf = get_cjson("./conf.json")) == NULL) {
        perror("get_conf");
        exit(1);
    }
    
    DBG(GREEN"Get cJSON conf success...\n"NONE);

    if ((slave = get_cjson("./slave_timer.json")) == NULL) {
        perror("get_slave");
        exit(1);
    }

    DBG(GREEN"Get cJSON slave success ...\n"NONE);
    MasterPort = get_json_valueint(conf, "Master", "MasterPort");
    strcpy(MasterDesc, get_json_valuestring(conf, "Master", "Desc"));
    
    DBG(GREEN"Get MasterPort ans MasterDesc success...\n"NONE);
    
    SlavePort = get_json_valueint(conf, "Slave", "SlavePort");
    strcpy(SlaveDesc, get_json_valuestring(conf, "Slave", "Desc"));
    
    pid_t pid;
    int x;
    for (int i = 1; i <= 2; i++) {
        if((pid = fork()) < 0) {
            perror("fork()");
            exit(1);
        }
        x = i;
        if (pid == 0) break;
    }
    if (pid) {
        DBG(RED"The %dth Process act as a Controler!\n"NONE, x);
        while (1) {
            int cmd;
            DBG(YELLOW"Please Input You Cmd(0 for Master, 1 for Slave):\n"NONE);
            scanf("%d", &cmd);
            if (cmd == 0) {
                share_memory->type = 0;
            } else {
                share_memory->type = 1;
            }
            pthread_cond_signal(&share_memory->cond);
        }
    } else if (pid == 0 && x == 1) {
        DBG(PINK"The %dth Process act as a Master!\n"NONE, x);
        while (1) {
            if (share_memory->type == 1) {
                pthread_mutex_lock(&share_memory->mutex);
                DBG(BLUE"Master Process Waiting for Signal...\n"NONE);
                while(share_memory->type == 1) {
                    pthread_cond_wait(&share_memory->cond, &share_memory->mutex);
                }
                pthread_mutex_unlock(&share_memory->mutex);
                DBG(GREEN"Master stop sleep, start while master!\n"NONE);
                while(1) {
                    if (share_memory->type == 1) {
                        DBG(RED"while Master end, start slave!\n"NONE);
                        break;
                    }
                    doing_process(0);
                    sleep(2);
                }
            } else {
                DBG(BLUE"Start while Master!\n"NONE);
                while (1) {
                    if (share_memory->type == 1) {
                        DBG(RED"while Master end, start slave!\n"NONE);
                        break;
                    }
                    doing_process(0);
                    sleep(2);
                }
            }
        }
    } else if (pid == 0 && x == 2) {
        DBG(BLUE"The %dth Process act as a Slave!\n"NONE, x);
        while (1) {
            if (share_memory->type == 0) {
                pthread_mutex_lock(&share_memory->mutex);
                DBG(BLUE"Slave Process Waiting for Signal...\n"NONE);
                while(share_memory->type == 0) {
                    pthread_cond_wait(&share_memory->cond, &share_memory->mutex);
                }
                pthread_mutex_unlock(&share_memory->mutex);
                DBG(GREEN"Slave stop sleep, start while slave\n"NONE);
                while(1) {
                    if (share_memory->type == 0) {
                        DBG(RED"while slave end, start Master\n"NONE);
                        break;
                    }
                    doing_process(1);
                    sleep(2);
                }
            } else {
                DBG(BLUE"Start while Salve!\n"NONE);
                while (1) {
                    if (share_memory->type == 0) {
                        DBG(RED"while slave end, start Master\n"NONE);
                        break;
                    }
                    doing_process(1);
                    sleep(2);
                }
            }
        }
    }

    return 0;
}
