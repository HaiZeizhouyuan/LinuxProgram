/*************************************************************************
	> File Name: game_ui.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 10时50分26秒
 ************************************************************************/

#ifndef _GAME_UI_H
#define _GAME_UI_H

#define MSG_WIDTH 80
#define MSG_HEIGHT 20
#define HELP_WIDTH 30
#define INPUT_HEIGHT 5

extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;

void gotoxy(int x, int y);
void gotoxy_putc(int x,int y, char c);
void gotoxy_puts(int x,int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x,int y, char c);
void w_gotoxy_puts(WINDOW *win, int x,int y, char *s);
void initfootball();
//void print_message(WINDOW *win, struct ChatMsg *msg, int type);

#endif
