/*************************************************************************
	> File Name: send_ctl.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月30日 星期五 15时16分06秒
 ************************************************************************/

#include "head.h"

extern int sockfd;
extern struct FootBallMsg ctl_msg;
extern struct Bpoint ball;
extern struct FootBallMsg chat_msg;
void send_ctl() {
    if (ctl_msg.ctl.dirx || ctl_msg.ctl.diry) {
        ctl_msg.ctl.action =  ACTION_DFL;
        send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
        ctl_msg.ctl.dirx = ctl_msg.ctl.diry = 0;
        show_data_stream('n');
        //char tmp[100] = {0};
        //sprintf(tmp, "%s[%d, %d],BALL[%d, %d]", chat_msg.name,  ctl_msg.ctl.dirx, ctl_msg.ctl.diry, (int)ball.x, (int)ball.y );
        //show_message(NULL, NULL, tmp, 1);
     //   client_re_draw();
    }
}
