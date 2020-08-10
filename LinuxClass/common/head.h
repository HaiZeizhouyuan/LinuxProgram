/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail: 
	> Created Time: Sun Jul 26 18:38:02 2020
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "color.h"
#include "common.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <common.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args);
#else 
#define DBG(fmt, args...) 
#endif

#endif

