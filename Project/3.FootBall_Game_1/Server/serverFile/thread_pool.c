/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu Aug  6 08:55:20 2020
 ************************************************************************/

#include "head.h"
extern int repollfd, bepollfd;
extern pthread_mutex_t red_mutex, blue_mutex;
extern struct Map court;
extern struct BallStatus ball_status;
extern int blue_num,  red_num;
extern struct Bpoint ball;
extern struct User *red_team, *blue_team;
char user_logout[512];

void map_change() {
    char map[1024] = {0};
    struct FootBallMsg chat_msg;
    sprintf(map, "%s", create_spirit());
    bzero(&chat_msg, sizeof(chat_msg));
    chat_msg.type = FT_MAP;
    strcpy(chat_msg.msg, map);
    chat_msg.size = sizeof(chat_msg.msg);
    send_all(&chat_msg);
}

void do_work(struct User *user) {
    struct FootBallMsg chat_msg;
    DBG(BLUE"Server Start Recv!\n"NONE);
    bzero(&chat_msg, sizeof(chat_msg));
    int ret = recv(user->fd, (void *)&chat_msg, sizeof(chat_msg), 0);
    if (ret != sizeof(chat_msg)) {
        perror("recvfrom()");
        return ;
    }
    DBG(BLUE"Have recv msg success!\n"NONE);
    //show_message(NULL,NULL, "have msg", 1);
    user->flag = 10;
    if (chat_msg.type & FT_ACK) {
        if (user->team) {
            DBG(L_BLUE" %s "NONE"❤\n", user->name);
        } else {
            DBG(L_RED" %s "NONE"❤\n", user->name);
        }
    } else if (chat_msg.type & FT_MSG) {//聊天
        show_message(NULL, user, chat_msg.msg, 0);
        send_all(&chat_msg); 
    } else if (chat_msg.type & FT_PRI) {
        if (user->team) {
            send_team(blue_team, &chat_msg);
        } else {
            send_team(red_team, &chat_msg);
        }
        show_message(NULL, user, chat_msg.msg, 2);
    }else if (chat_msg.type & FT_FIN) {
        show_data_stream('e');
        chat_msg.type = FT_WALL;
        memset(chat_msg.msg, 0, sizeof(chat_msg.msg));
        char tmp[512] = {0};
        sprintf(tmp, "Your good friend %s have logout!", user->name);
        strcpy(chat_msg.msg, tmp);
        send_all(&chat_msg);
        user->online = 0;
        if (user->team == 1) red_num -= 1;
        else blue_num -= 1;
        //加锁
        if (user->team == 1) pthread_mutex_lock(&blue_mutex);
        else pthread_mutex_lock(&red_mutex);
        int tmp_epollfd = ( user->team ? bepollfd : repollfd);
        del_event(tmp_epollfd, user->fd);
        close(user->fd);
        show_message(NULL, NULL, tmp, 1);
        if (user->team == 1) pthread_mutex_unlock(&blue_mutex);
        else pthread_mutex_unlock(&red_mutex);
        map_change();
    } else if (chat_msg.type & FT_CTL) {//玩家的移动
        char tmp[512] = {0};
        show_message(NULL, user, tmp, 0);
        if (chat_msg.ctl.dirx * ((int)ball.x - user->loc.x + 2) < 0) user->carry_flag = 0;
        if (chat_msg.ctl.diry * ((int)ball.y - user->loc.y + 2) < 0) user->carry_flag = 0;
        if (chat_msg.ctl.action & ACTION_DFL) {
            show_data_stream('n');
            if (user->carry_flag == 1) {
                if ((int)ball.x > user->loc.x - 2) ball.x = user->loc.x + chat_msg.ctl.dirx - 2 + 1;
                else if ((int)ball.x < user->loc.x - 2) ball.x = user->loc.x + chat_msg.ctl.dirx - 2 - 1;
                else ball.x = user->loc.x - 2;
                if ((int)ball.y > user->loc.y - 1 ) ball.y = user->loc.y + chat_msg.ctl.diry - 1 + 1;
                else if ((int)ball.y < user->loc.y - 1) ball.y = user->loc.y + chat_msg.ctl.diry - 1 + - 1;
                else ball.y = user->loc.y - 1;
            }
            user->loc.x += chat_msg.ctl.dirx;
            user->loc.y += chat_msg.ctl.diry;
            //玩家移动的边界
            if (user->loc.x <= 0) user->loc.x = 0;
            if (user->loc.x >= court.width + 3) user->loc.x = court.width + 3;
            if (user->loc.y <= 0) user->loc.y = 0;
            if (user->loc.y >= court.height + 1) user->loc.y = court.height + 1;
            map_change();
        } else if (chat_msg.ctl.action & ACTION_KICK) {//踢球
            sprintf(tmp, "%s kicks ball with %d Newtons of force!", user->name, chat_msg.ctl.strength);
            show_data_stream('k');
            user->carry_flag = 0;
            if (can_kick(&user->loc, chat_msg.ctl.strength)) {
                sprintf(tmp, "ball(%lf, %lf), %s(%d, %d)\n",  ball.x, ball.y, user->name, user->loc.x, user->loc.y);
                show_message(NULL, NULL, tmp, 1);
                ball_status.by_team = user->team;
                strcpy(ball_status.name, user->name);
                sprintf(tmp, " vx = %f, vy = %f, ax = %f, ay = %f\n",ball_status.v.x, ball_status.v.y, ball_status.a.x, ball_status.a.y);
               // show_message(NULL, NULL, tmp, 1);          
            }
        } else if (chat_msg.ctl.action & ACTION_STOP) {
            user->carry_flag = 0;
            show_data_stream('s');
            if (can_access(&user->loc)) {
                bzero(&ball_status.v, sizeof(ball_status.v));
                bzero(&ball_status.a, sizeof(ball_status.a));
                sprintf(tmp, "Stop The Ball!");
                show_message(NULL, user, tmp, 0);
            }
        } else  if (chat_msg.ctl.action & ACTION_CARRY){
            show_data_stream('c');
            sprintf(tmp, "Try Carry The Ball!");
            if (can_access(&user->loc)) user->carry_flag = 1;
            show_message(NULL, user, tmp, 0);
        }
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

