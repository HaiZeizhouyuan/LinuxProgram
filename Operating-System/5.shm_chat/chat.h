/*************************************************************************
	> File Name: chat.h
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 09:20:57 2020
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H
#include "head.h"
struct Message {
    char name[20];
    char msg[1024];
    pid_t pid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
#endif
