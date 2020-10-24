/*************************************************************************
	> File Name: get_info.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月19日 星期一 13时36分50秒
 ************************************************************************/

#ifndef _GET_INFO_H
#define _GET_INFO_H
void *get_info(void *arg);
int run_and_save(char *cmd, char *file_name, int sleeptime);
void send_info(int sockfd, FILE *filename);
#endif
