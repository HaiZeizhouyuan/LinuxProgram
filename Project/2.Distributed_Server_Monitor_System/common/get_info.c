/*************************************************************************
	> File Name: get_info.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月19日 星期一 13时34分14秒
 ************************************************************************/
#include "head.h"
extern cJSON *slave;
extern int sockfd;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *get_info(void *arg) {
    pthread_mutex_lock(&mutex);
    int num = *(int *)arg;
    char info[10], cmd[20];
    char file_path[20];
    
    if (num == 0) {
        strcpy(info, "Proc");
    } else if (num == 1) {
        strcpy(info, "Disk");
    } else if (num == 2) {
        strcpy(info, "Mem");
    } else if (num == 3) {
        strcpy(info, "Cpu");
    } else if (num == 4) {
        strcpy(info, "User");
    } else {
        strcpy(info, "Sysinfo");
    }

    sprintf(cmd, "bash ../Script/%s.sh", info);
    sprintf(file_path, "./SysInfo/%s%s", info, ".txt");

    int sleeptime = 0;
    sleeptime = get_timer(slave, info);
    DBG(BLUE"<%ld> : %s sleeptime is %d\n"NONE, pthread_self(), info, sleeptime);
    pthread_mutex_unlock(&mutex);
    if (run_and_save(cmd, file_path, sleeptime) == -1) {
        return NULL;
    }
    return NULL;
}

int run_and_save(char *cmd, char *file_path, int sleeptime) {
    FILE *filename = NULL;
    char shell_cmd[50];
    sprintf(shell_cmd, "%s >> %s", cmd, file_path);
    if ((filename = popen(shell_cmd, "w")) == NULL) {
        pclose(filename);
        perror("fopen()");
        return -1;
    }
    pclose(filename);
    DBG(RED"run %s and save %s is success!\n"NONE, cmd, file_path);
   // send_info(sockfd, filename);
    sleep(sleeptime);
    return 0;

}

void send_info(int sockfd, FILE *filename) {
    char temp[1024], buff[1024];
    while((fgets(temp, 1024, filename)) != NULL) {
        sprintf(buff, "%s%s", buff, temp);
        memset(temp, 0, sizeof(temp));
    }
    send(sockfd, (void *)&buff, sizeof(buff), 0);
    return ;
}
