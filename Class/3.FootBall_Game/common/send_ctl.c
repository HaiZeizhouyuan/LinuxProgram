/*************************************************************************
	> File Name: send_ctl.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月30日 星期五 15时16分06秒
 ************************************************************************/

#include "head.h"

extern int sockfd;
extern struct FootBallMsg ctl_msg;

void send_ctl() {
    if (ctl_msg.ctl.dirx || ctl_msg.ctl.diry) {
        ctl_msg.ctl.action =  ACTION_DFL;
        send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
        ctl_msg.ctl.dirx = ctl_msg.ctl.diry = 0;
      //  show_data_stream('n');

    }
}
