/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "head.h"

extern int epollfd, red_epollfd, blue_epollfd;
extern struct User *users;
extern int maxfd;
void do_work(struct User *user) {
    struct FootBallMsg msg;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    DBG(BLUE"Server Start Recv!\n"NONE);
    bzero(&msg, sizeof(msg));
    int ret = recv(user->fd, (void *)&msg, sizeof(msg), 0);
    if (ret != sizeof(msg)) {
        perror("recvfrom()");
        return ;
    }
    DBG(BLUE"Have recv msg success!\n"NONE);
    user->flag = 10;
    if (msg.type & FT_WALL) {
        DBG(BLUE"[ WALL ]"NONE" : %s say %s\n", msg.name, msg.msg);
        send_all(&msg);
        
    } else if (msg.type & FT_MSG) {
        DBG(BLUE"[TEAM]"NONE" : %s say %s \n", msg.name, msg.msg);
        if (user->team) send_team(blue_team, &msg);
        else send_team(red_team, &msg);
    } else if (msg.type & FT_FIN) {
        DBG(L_PINK"Chat Fin"NONE " :recv a CHAT_FIN from %s\n", user->name);
        send(user->fd, (void *)&msg, sizeof(msg), 0);
        DBG(L_PINK"Chat Fin"NONE" : send a CHAT_FIN_1 to %s\n", user->name);
        user->online = 0;
        //加锁
        int tmp_epollfd = ( user->team ? blue_epollfd : red_epollfd);
        del_event(tmp_epollfd, user->fd);
        close(user->fd);
    }
    
}

void task_queue_init(struct task_queue *taskQueue, int size, int epollfd) {
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(size, sizeof(struct User));
    taskQueue->head = taskQueue->tail = taskQueue->total = 0;
    taskQueue->size = size;
    //对互斥锁的初始化,使用默认锁属性
    pthread_mutex_init(&taskQueue->mutex, NULL);//线程的锁不需要进程的共享
    //对变量的初始化
    pthread_cond_init(&taskQueue->cond, NULL);
    DBG(GREEN"<INIT>"NONE"queue have init!\n");
    return ;
}

void task_queue_push(struct task_queue *taskQueue,struct User *user) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) {
        pthread_mutex_unlock(&taskQueue->mutex);
        DBG(YELLOW"<Debug>"NONE" : taskQueue is full!\n");
        return ;
    }
    taskQueue->team[(taskQueue->tail)++] = user;
    DBG(GREEN"<Push>"NONE" : %s\n", user->name);
    if (taskQueue->tail == taskQueue->size) {
        DBG(RED"<taskQueue>"NONE ": End\n ");
        taskQueue->tail = 0;
    }
    taskQueue->total += 1;
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
    return ;
}

int empty(struct task_queue *taskQueue) {
    return taskQueue->total == 0;
}

struct User *front (struct task_queue *taskQueue) {
    return taskQueue->team[taskQueue->head];
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    //惊群效应
    while (empty(taskQueue)) {
        DBG(PINK"<Dbug>"NONE" : taskQueue is empty!\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);//解锁，等待信号
    }
    struct User *user = taskQueue->team[taskQueue->head];
    taskQueue->total -= 1;
    DBG(BLUE"<Pop> "NONE" : pop %s!\n", user->name);
    if (++taskQueue->head == taskQueue->size) taskQueue->head = 0;
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run (void *arg) {
    DBG(RED"START thread_run!\n"NONE);
    pthread_detach(pthread_self());//分离线程,其他线程不用在等待
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_work(user);
    }
}

