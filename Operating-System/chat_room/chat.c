/*************************************************************************
	> File Name: chat.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月18日 星期二 09时14分26秒
 ************************************************************************/
#include "work_list.h"
#define BUFFSIZE 512
#define MAX_NUM 100
char buff[BUFFSIZE] = {0};
char root[20] = {"kk"};

char sign_success[20] = {"Sign in success!"};
char sys_bug[20] = {"sys have logout!"};

char now_name[20];
char have_sign[30];
char logoutname[30];
char name_and_msg[100];
int user_num = 1, sum = 0;
ChatMsg chatmsg;

void do_work(int fd) {
    DBG(RED"<FD>"NONE": %d\n", fd);
    memset(chatmsg.name, 0, sizeof(chatmsg));
    memset(chatmsg.msg, 0, sizeof(chatmsg));
    ssize_t rev;
    rev = recv(fd, &chatmsg, sizeof(chatmsg), 0);
    if (rev < 0) {
		perror("recv()");
		exit(1);
    }
    DBG(GREEN"<INFORMATION>"NONE"chat.name : %s, msg = %s, opt = %d\n", chatmsg.name, chatmsg.msg, chatmsg.opt);

    if (chatmsg.opt & CHAT_SYN) {
        chatmsg.opt |= ~CHAT_SYN;
        DBG(PINK"<Dbug>"NONE":welcome!\n");
        is_welcome(fd);
        return ; 
    }
    if (chatmsg.opt & CHAT_FIN) {
        DBG(YELLOW"<Dbug>"NONE" : logout!\n");
        tell_sys_logout(fd);
    }
    DBG(RED"<Name>"NONE": %s "RED"<Msg>"NONE": %s\n", chatmsg.name, chatmsg.msg);
        /*if (strncmp(chatmsg.msg, '@', 1) == 0) {
            for (int i = 0; ) {

            }
            for (int i = 1; i <= user_num; i++) {
                if (user_num[i].online == 1) {
                }
            }
            private_chat();
        } else */
    if (strcmp(chatmsg.msg, "#1") == 0) {
            
    } else {      
        sprintf(name_and_msg, "%s:%s", chatmsg.name, chatmsg.msg);
        strncpy(chatmsg.msg, name_and_msg, strlen(name_and_msg));
        DBG(YELLOW"<chatmsg.msg>"NONE":%s\n", chatmsg.msg);
        for (int i = 1; i <= user_num; i++) {
            if (users[i].online == 1) {
                DBG(BLUE"<Dbug>"NONE"users[%d].fd = %d\n", i, users[i].fd);
                send(users[i].fd, &chatmsg, sizeof(chatmsg), 0);
            } 
        }
    }
    sleep(1);
    return ;
}

/*void Stop(int signal) {
    DBG(RED"<Dbug>"NONE":have stop\n");
    for (int i = 1; i <= user_num; i++) {
        if (users[i].online == 1){
            memset(chatmsg.msg, 0, sizeof(chatmsg.msg));
            memset(chatmsg.name, 0, sizeof(chatmsg.name));
            strncpy(chatmsg.name, root, strlen(root));
            strncpy(chatmsg.msg, sys_bug, strlen(sys_bug));
            chatmsg.opt |= CHAT_FIN_1;
            send(users[i].fd, &chatmsg.msg, sizeof(chatmsg.msg), 0);
            close(users[i].fd);
        }
    }
    exit(0);
};*/

void sendmsg_to_every() {

}

void tell_sys_logout(int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
    memset(chatmsg.msg, 0, sizeof(chatmsg.msg));
    chatmsg.opt |= ~CHAT_FIN;
    chatmsg.opt |= CHAT_FIN_1;
    send(fd, &chatmsg, sizeof(chatmsg), 0);
    close(fd);
    printf("%s have logout!\n", chatmsg.name);
    for (int i = 1; i <= user_num; i++) {
        if ((strcmp(users[i].name, chatmsg.name)) == 0) {
            strncpy(users[i].name, root, strlen(root));
            users[i].online = 0;
            user_num -= 1;
        }
    }
    return ;
}
void is_welcome(int fd) {
    for (int i = 1; i <= user_num; i++) {
        if ((strcmp(users[i].name, chatmsg.name)) == 0) {
            memset(chatmsg.name, 0, sizeof(chatmsg));
            memset(chatmsg.msg, 0, sizeof(chatmsg));
            DBG(YELLOW"<Dbug>"NONE":have sign in!\n");
            sprintf(chatmsg.msg, "%s have sign in!", users[i].name);
            strncpy(chatmsg.name, root, strlen(root));
            send(fd, &chatmsg, sizeof(chatmsg), 0);
            close(fd);
            return ;
        }
    }
    user_num += 1;
    memset(users[user_num].name, 0, strlen(users[user_num].name));
    DBG(BLUE"<Dbug>"NONE"memset name : %s\n", users[user_num].name);
    for (int i = 1; i < MAX_NUM; i++) {
        if (users[i].online == 0) {
            strcpy(users[i].name, chatmsg.name);
            users[i].fd = fd;
            users[i].online = 1;
            strncpy(now_name, users[i].name, strlen(users[i].name));
            break;
        }
    }
    strncpy(now_name, chatmsg.name, strlen(chatmsg.name));
    memset(chatmsg.name, 0, sizeof(chatmsg));
    memset(chatmsg.msg, 0, sizeof(chatmsg));
    strncpy(chatmsg.name, root, strlen(root));
    strncpy(chatmsg.msg, sign_success, strlen(sign_success));
    send(fd, &chatmsg, sizeof(chatmsg), 0);
    memset(chatmsg.msg, 0, sizeof(chatmsg));
    sprintf(chatmsg.msg, "Welcome %s!", now_name);
    for (int i = 1; i <= user_num; i++) {
        if (users[i].online == 1) {
            DBG(BLUE"<FD>"NONE": %d : %s\n", users[i].fd, chatmsg.msg);
            send(users[i].fd, &chatmsg, sizeof(chatmsg), 0);
        }
    }
    return ;
}

void user_init(User *users) {
    for (int i = 1; i < MAX_NUM; i++) {
        strcpy(users[i].name, root);
        users[i].online = 0;
    }
}

