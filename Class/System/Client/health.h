/*************************************************************************
	> File Name: health.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月16日 星期五 18时45分48秒
 ************************************************************************/

#ifndef _HEALTH_H
#define _HEALTH_H
struct Share{
    int type;//0 Master启动，１salve启动
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
#endif
