/*************************************************************************
	> File Name: thread_pool.c
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 2020年06月07日 星期日 20时05分50秒
 ************************************************************************/
#include "head.h"

extern int repollfd, bepollfd;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Map court;
extern pthread_mutex_t red_mutex, blue_mutex;

void do_work(struct User *user) {
    struct FootBallMsg msg;
    char tmp[512] = {0};
    int size = recv(user->fd, (void *)&msg, sizeof(msg), 0);
    if (size <= 0) {
        perror("recv()");
        return ;
    }
    user->flag = 10;
    if (msg.type & FT_ACK) {
        if (user->team) {
            DBG(L_BLUE" %s "NONE"❤\n", user->name);
        } else {
            DBG(L_RED" %s "NONE"❤\n", user->name);
        }
    } else if (msg.type & (FT_WALL | FT_MSG)) {
        if (user->team) {
            DBG(L_BLUE" %s : %s"NONE, user->name, msg.msg);
        } else {
            DBG(L_RED" %s : %s"NONE, user->name, msg.msg);
        }
        strcpy(msg.name, user->name);
        msg.team = user->team;
        show_message(NULL, user, msg.msg, 0);
        send_all(&msg);
    } else if (msg.type & FT_FIN) {
        if (user->team) pthread_mutex_lock(&blue_mutex);
        else pthread_mutex_lock(&red_mutex);
        show_data_stream('e');
        DBG(RED"%s logout.\n", user->name);
        sprintf(tmp, "%s Logout.", user->name);
        show_message(NULL, NULL, tmp, 1);
        user->online = 0;
        int epollfd_tmp = (user->team ? bepollfd : repollfd);
        del_event(epollfd_tmp, user->fd);
        close(user->fd);
        char tmp[512] = {0};
        sprintf(tmp, "%s have logout!", user->name);
        memset(msg.msg, 0, sizeof(msg.msg));
        msg.type = FT_WALL;
        send_all(&msg);
        if (user->team) pthread_mutex_unlock(&blue_mutex);
        else pthread_mutex_unlock(&red_mutex);

    } else if (msg.type == FT_CTL) {
        char tmp[512] = {0};
        show_message(NULL, user, "Ctrl Message", 0);
        if (msg.ctl.action & ACTION_DFL){
            show_data_stream('n');
            user->loc.x += msg.ctl.dirx;
            user->loc.y += msg.ctl.diry;
            if (user->loc.x <= 1) user->loc.x = 1;
            if (user->loc.x >= court.width + 2) user->loc.x = court.width + 2;
            if (user->loc.y <= 0) user->loc.y = 0;
            if (user->loc.y >= court.height + 1) user->loc.y = court.height + 1;
        } else if (msg.ctl.action & ACTION_KICK) {
            show_data_stream('k');
            if (can_kick(&user->loc, msg.ctl.strength)) {
                ball_status.by_team = user->team;
                strcpy(ball_status.name, user->name);
            }
            strcpy(tmp, "Kick The Ball");
        } else if (msg.ctl.action & ACTION_STOP) {
            show_data_stream('s');
            if (can_access(&user->loc)) {
                bzero(&ball_status.v, sizeof(ball_status.v));
                bzero(&ball_status.a, sizeof(ball_status.a));
                strcpy(tmp,"Stop The Ball");
            }
        } else if (msg.ctl.action & ACTION_CARRY) {
            show_data_stream('c');
            sprintf(tmp, "Try  Carry The Ball");
            if (ball.x > user->loc.x) ball.x  = user->loc.x + 1;
            else ball.x  = user->loc.x - 1;
            if (ball.y > user->loc.y) ball.y = user->loc.y + 1;
            else ball.y = user->loc.y - 1;
        }
        char temp[512] = {0};
        sprintf(temp, "%s, p(%d %d), b(%lf, %lf)", tmp, user->loc.x, user->loc.y, ball.x, ball.y);
        show_message(NULL, NULL, temp, 1);
    }
}

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd) {
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *));
    taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
}

void task_queue_push(struct task_queue *taskQueue, struct User *user) {
    pthread_mutex_lock(&taskQueue->mutex);
    taskQueue->team[taskQueue->tail] = user;
    DBG(L_GREEN"Thread Pool :"NONE" Task Push %s\n", user->name);
    if (++taskQueue->tail == taskQueue->sum) {
        DBG(L_GREEN"Thread Pool :"NONE" Task Queue End.\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    while (taskQueue->tail == taskQueue->head) {
        DBG(L_GREEN"Thread Pool :"NONE" Task Queue Empty, Waiting For Task.\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    struct User *user = taskQueue->team[taskQueue->head];
    DBG(L_GREEN"Thread Pool :"NONE" Task Pop %s.\n", user->name);
    if (++taskQueue->head == taskQueue->sum) {  
        DBG(L_GREEN"Thread Pool :"NONE" Task Queue End.\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run(void *arg) {
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_work(user);
    }
}
