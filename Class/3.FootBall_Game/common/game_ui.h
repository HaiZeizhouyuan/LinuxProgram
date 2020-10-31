/*************************************************************************
	> File Name: game_ui.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月28日 星期三 10时50分26秒
 ************************************************************************/

#ifndef _GAME_UI_H
#define _GAME_UI_H

//extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;


WINDOW *create_newwin(int width, int height, int startx, int starty);
void destroy_win(WINDOW *win);
void gotoxy(int x, int y);
void gotoxy_putc(int x,int y, char c);
void gotoxy_puts(int x,int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x,int y, char c);
void w_gotoxy_puts(WINDOW *win, int x,int y, char *s);
void initfootball();
void show_message(WINDOW *win, struct User *user, char *msg, int type);

#endif
