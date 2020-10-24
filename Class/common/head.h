/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail: 
	> Created Time: Sun Jul 26 18:38:02 2020
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <math.h>
#include <mntent.h>
#include <ncurses.h>
#include <locale.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/statfs.h>
#include "color.h"
#include "common.h"
#include "datatype.h"
#include "chat.h"
#include "thread_pool.h"
#include "client_recv.h"
#include "chat_ui.h"
#include "file.h"
#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args); //将后面的参数加到后面，　如果没有参数则将'.'删除
#else 
#define DBG(fmt, args...)  //如果定义了，预编译时会删掉这个宏　
#endif

#endif

