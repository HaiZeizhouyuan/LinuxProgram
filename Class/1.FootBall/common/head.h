/*************************************************************************
	> File Name: ../common/head.h
	> Author: wangshuai
	> Mail: ws1519704327@126.com
	> Created Time: 六  3/28 14:23:19 2020
 ************************************************************************/

#ifndef HEAD_H
#define HEAD_H

#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <mntent.h>
#include <ncurses.h>
#include <locale.h>
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
#include "common.h"
#include "color.h"
#include "datatype.h"
#include "cJSON.h"
#include "get_cjson_value.h"
#include "heart_beat.h"
#include "thread_pool.h"
#include "server_exit.h"
#include "client_recver.h"
#include "game_ui.h"
#include "udp_epoll.h"
#include "server_send_all.h"
#include "sub_reactor.h"
#include "send_ctl.h"
#include "server_exit.h"
#include "send_chat.h"
#include "show_strength.h"
#include "show_data_stream.h"
#include "ball_status.h"
#include "server_re_drew.h"
#include "can_access.h"
#include "create_spirit.h"


#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args);
#else 
#define DBG(fmt, args...) 
#endif


#endif