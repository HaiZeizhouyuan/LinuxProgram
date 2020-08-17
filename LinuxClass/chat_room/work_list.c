/*************************************************************************
	> File Name: work_list.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "work_list.h"
#define BUFFSIZE 512

extern int epollfd;
extern User user;
char buff[BUFFSIZE] = {0};
int user_num = 0, sum = 0;

void do_work(int fd) {
    memset(buff, 0, strlen(buff));
    ssize_t rev;
    rev = recv(fd, buff,sizeof(buff), 0);
    if (rev < 0) {
		perror("recv()");
		exit(1);
    }

	if (rev == 0) {
    	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
        DBG(RED"<Reactor>"NONE" : Del from reactor!\n");
   		close(fd);
        printf("have logout!\n");
        return ;
    }

    char str[10] = {"Welcome"};
    char sign_success[20] = {"Sign in success!"};
    char have_sign[30];
    int is_find = 0;
    DBG(BLUE"<Dbug>"NONE"str : %s, buff: %s\n", str, buff);
    if (strncmp(buff, str, 7) == 0) {
        for (int i = 1; i <= user_num; i++) {
            DBG(GREEN"<Dbug>"NONE"user_name :%s, buff + 8: %s\n", users[i].name, buff + 8);
            if ((strcmp(users[i].name, buff + 8)) == 0) {
                sprintf(have_sign, "%s have sign in!\n", users[i].name);
                send(fd, have_sign, strlen(have_sign), 0);
                close(fd);
                return ;
            }
        }
        strcpy(users[++user_num].name, buff + 8);
        printf("%s\n", buff);
        send(fd, sign_success, strlen(sign_success), 0);
        users[user_num].online = 1;
    } else {
        printf("%s\n", buff);
    }
    //printf("name:%s\n", users[user_num].name);
    sleep(1);
}
void task_queue_init(Task_Queue *taskQueue, int size) {
    taskQueue->data = calloc(size, sizeof(int));
    taskQueue->head = taskQueue->tail = taskQueue->total = 0;
    taskQueue->size = size;
    //对互斥锁的初始化,使用默认锁属性
    pthread_mutex_init(&taskQueue->mutex, NULL);//线程的锁不需要进程的共享
    //对变量的初始化
    pthread_cond_init(&taskQueue->cond, NULL);
    return ;
}

void expend(Task_Queue *taskQueue) {
    DBG(GREEN"<Dbug>"NONE ": start expend!\n");
    int extr_size = taskQueue->size;
    int *p;
    while (extr_size) {
        p = (int *)malloc(sizeof(int) * (taskQueue->size + extr_size));
        if (p) break;
        extr_size >>= 1;
    }
    for (int i = taskQueue->head, j = 0; j < taskQueue->total; j++ ) {
        p[j] = taskQueue->data[(i + j) % taskQueue->size];
    }

    free(taskQueue->data);
    taskQueue->data = p;
    taskQueue->head = 0;
    taskQueue->tail = taskQueue->total;
    taskQueue->size += extr_size;
    DBG(GREEN"<Dbug>"NONE" : expend success!\n");
    return ;

}

void task_queue_push(Task_Queue *taskQueue, int data) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) {
        pthread_mutex_unlock(&taskQueue->mutex);
        DBG(YELLOW"<Debug>"NONE" : taskQueue is full!\n");
        return ;
    }
    taskQueue->data[(taskQueue->tail)++] = data;
    DBG(GREEN"<Push>"NONE" : %d\n", data);
    if (taskQueue->tail == taskQueue->size) {
        DBG(RED"<taskQueue>"NONE ": End\n ");
        taskQueue->tail -= taskQueue->size;
    }
    taskQueue->total += 1;
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
    return ;
}

int empty(Task_Queue *taskQueue) {
    return taskQueue->total == 0;
}

int front (Task_Queue *taskQueue) {
    return taskQueue->data[taskQueue->head];
}

int task_queue_pop(Task_Queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    //惊群效应
    while (empty(taskQueue)) {
        DBG(PINK"<Dbug>"NONE" : taskQueue is empty!\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);//解锁，等待信号
    }
    int f = taskQueue->data[taskQueue->head];
    taskQueue->total -= 1;
    DBG(BLUE"<Pop> "NONE" : pop %d!\n", f);
    if (++taskQueue->head == taskQueue->size) taskQueue->head -= taskQueue->size;
    pthread_mutex_unlock(&taskQueue->mutex);
    return f;
}

void clear(Task_Queue *taskQueue) {
    if (taskQueue == NULL) return ;
    free(taskQueue->data);
    free(taskQueue);
    return ;
}

void *thread_run (void *arg) {
    pthread_detach(pthread_self());
    Task_Queue *taskQueue = (Task_Queue *)arg;
    while (1) {
        int fd = task_queue_pop(taskQueue);
        do_work(fd);
    }
}

