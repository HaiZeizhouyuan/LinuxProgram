/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "head.h"

extern int epollfd;
extern struct User *users;
extern int maxfd;
extern int cnt_online;
char niming[20] = {"lplp"};
void get_name(int fd) {
    struct ChatMsg msg;
    memset(&msg,0,sizeof(msg));
    msg.type = CHAT_SYS;
    int cnt = 0;
    for (int i = 1; i <= maxfd; i++) {
        if (cnt > 10) break;
        if (users[i].online == 1) {
            strcat(msg.msg, users[i].chat_name);
            strcat(msg.msg, ",");
            cnt++;
        }
    }
    msg.msg[strlen(msg.msg) - 1] = ' ';
    char tmp[100];
    sprintf(tmp, "等%d位好友在线!\n", cnt_online);
    strcat(msg.msg, tmp);
    send(fd, (void *)&msg, sizeof(msg), 0);
}

void send_all(struct ChatMsg *msg) {
    for (int i = 1; i <= maxfd; i++) {
        if (users[i].online == 1) {
            int a = 0;
            DBG(RED"have send%d"NONE"%s\n", a++, msg->msg);
            send(users[i].fd, (void *)msg, sizeof(struct ChatMsg), 0);
        }
    }
}

void send_to_name(char *to, struct ChatMsg *msg, int fd) {
    DBG(RED"START send_to_name"NONE);
    int flag = 0;
    for (int i = 1; i <= maxfd; i++) {
        if (users[i].online && !strcmp(to, users[i].chat_name)) {
            send(users[i].fd, (void *)msg, sizeof(msg), 0);
            flag = 1;
            break;
        }
    }
    if (!flag) {
        memset(msg->msg, 0, sizeof(msg->msg));
        sprintf(msg->msg, "用户%s不在线或者用户名错误!\n", to);
        msg->type = CHAT_SYS;
        send(fd, (void *)msg, sizeof(struct ChatMsg), 0);
    }
}

void send_to (int fd, struct ChatMsg *msg) {
    DBG(RED"START send_to!\n"NONE);
    char to_name[20] = {0};
    int i = 1; //@zhouyuan 你好
    for (; i <= 21; i++) {
        if (msg->msg[i] == ' ') break;
    }
    if (msg->msg[0] != '@' || msg->msg[i] != ' ') {
        memset(msg, 0, sizeof(struct ChatMsg));
       msg->type = CHAT_SYS;
        sprintf(msg->msg, "私聊信息格式错误!\n");
        send(fd, (void *)msg, sizeof(struct ChatMsg), 0);
    } else {
        strncpy(to_name, msg->msg + 1, i - 1);
        send_to_name(to_name, msg, fd);

    }
}

void do_work (struct User *user) {
    DBG(RED"START do_work!\n"NONE);
    struct ChatMsg msg;
    int re;
    /*if ((re = recv(user->fd, &msg, sizeof(msg), 0)) <= 0) {
        DBG(L_PINK"thread_pool"NONE" : %s closed connection abnormsally!, re = %d\n", user->chat_name, re);
        del_event(epollfd, user);
        cnt_online--;
        close(user->fd);
        return ;
    }*/
    DBG(RED"server start recv!\n"NONE);
    recv(user->fd, &msg, sizeof(msg), 0);
    DBG(RED"mag:%d\n"NONE, msg.type);
    user->flag = 10;
    //strcpy(user->chat_name, msg.name);
    DBG(RED"Start select type!\n"NONE);
    if (msg.type & CHAT_PUB) {
        DBG(RED"have pub msg!\n"NONE);
        strcpy(msg.name, user->chat_name);
        send_all(&msg);
        printf(BLUE"%s"NONE" : %s\n", user->chat_name, msg.msg);
    } else if (msg.type & CHAT_PRI) {
        strcpy(msg.name, user->chat_name);
        printf(PINK"%s"NONE" : %s *\n", user->chat_name, msg.msg);
        send_to(user->fd, &msg);
    } else if (msg.type & CHAT_SYN) {
        DBG(PINK"Is sign in!\n"NONE);
        do_login(user, &msg);
    } else if (msg.type & CHAT_FIN) {
        DBG(L_PINK"Chat Fin"NONE " :recv a CHAT_FIN from %s\n", user->chat_name);
        msg.type = CHAT_FIN_1;
        send(user->fd, (void *)&msg, sizeof(msg), 0);
        DBG(L_PINK"Chat Fin"NONE" : send a CHAT_FIN_1 to %s\n", user->chat_name);
        close(user->fd);
        user->online = 0;
        del_event(epollfd, user);
        cnt_online --;
    } else if (msg.type & CHAT_FUNC) {
        DBG(YELLOW"type : chat_func\n"NONE);
        if (msg.msg[0] != '#') {
            sprintf(msg.msg, "请使用#+数字的形式使用聊天功能，#1查询在线人数，#2切换匿名!\n");
            msg.type = CHAT_SYS;
            send(user->fd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.msg[1] == '1') {
            get_name(user->fd);
        } else if (msg.msg[1] == '2') {
           strcpy(user->chat_name, user->name);
        }
    }
}

void task_queue_init(struct task_queue *taskQueue, int size, int epollfd) {
    taskQueue->epollfd = epollfd;
    taskQueue->users = calloc(size, sizeof(struct User));
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
    taskQueue->users[(taskQueue->tail)++] = user;
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
    return taskQueue->users[taskQueue->head];
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    //惊群效应
    while (empty(taskQueue)) {
        DBG(PINK"<Dbug>"NONE" : taskQueue is empty!\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);//等待信号, 当有信号时上锁
    }
    struct User *user = taskQueue->users[taskQueue->head];
    taskQueue->total -= 1;
    DBG(BLUE"<Pop> "NONE" : pop %s!\n", user->name);
    if (++taskQueue->head == taskQueue->size) taskQueue->head = 0;
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run (void *arg) {
    DBG(RED"START thread_run!\n"NONE);
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_work(user);
    }
}

