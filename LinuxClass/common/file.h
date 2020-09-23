/*************************************************************************
	> File Name: file.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月19日 星期六 10时52分03秒
 ************************************************************************/

#ifndef _FILE_H
#define _FILE_H
int send_file(const char *filename, int sockfd);
void recv_file(int sockfd);
#endif
