/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  8 14:31:10 2020
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

void setnonblocking(int fd);
void setblocking(int fd);

int socket_create_udp(int port);
int socket_udp();
#endif
