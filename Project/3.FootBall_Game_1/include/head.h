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
#include "datatype.h"
#include "cJSON.h"
#include "color.h"
#include "common.h"
#include "get_cjson_value.h"
#include "udp_epoll.h"
#include "sub_reactor.h"
#include "thread_pool.h"
#include "heart_beat.h"
#include "client_recv.h"
#include "game_ui.h"
#include "server_exit.h"
#include "server_send_all.h"
#include "client_exit.h"
#include "send_chat.h"
#include "send_ctl.h"
#include "show_strength.h"
#include "ball_status.h"
#include "can_access.h"
#include "server_re_draw.h"
#include "create_spirit.h"
#include "show_data_stream.h"
#include "parse_spirit.h"
#include "client_re_draw.h"
#include "get_map_value.h"
#include "court_draw.h"
#include "game_over.h"
#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args);
#else 
#define DBG(fmt, args...) 
#endif

#endif

