/*************************************************************************
	> File Name: create_spirit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月01日 星期日 18时12分38秒
 ************************************************************************/

#include "head.h"
#define MAX_TEAM 11
extern struct User *blue_team;
extern struct User *red_team;
extern struct BallStatus ball_status;
extern struct Bpoint ball;

char *string = NULL;
cJSON *spirit = NULL;
cJSON *red = NULL;
cJSON *blue = NULL;
cJSON *ball_spirit = NULL;
cJSON *player = NULL;
cJSON *name = NULL;
cJSON *who = NULL;
cJSON *x = NULL;
cJSON *y = NULL;

int add_player_to_team(cJSON *team, struct User *user) {
    char tmp[512] = {0};
    sprintf(tmp, "x = %d, y = %d", user->loc.x, user->loc.y);
   // show_message(NULL, NULL, tmp, 1);
    if ((player = cJSON_CreateObject()) == NULL) return -1;
    cJSON_AddItemToArray(team, player);
    if ((x = cJSON_CreateNumber(user->loc.x)) == NULL) return -1;
    if ((y = cJSON_CreateNumber(user->loc.y)) == NULL) return -1;
    if ((name = cJSON_CreateString(user->name)) == NULL) return -1;
    cJSON_AddItemToObject(player, "x", x);
    cJSON_AddItemToObject(player, "y", y);
    cJSON_AddItemToObject(player, "name", name);
    sprintf(tmp, "name = %s, x = %d, y = %d", name->valuestring, x->valueint, y->valueint);
   // show_message(NULL, NULL, tmp, 1);
    return 0;
}

char  *create_spirit() {
    if ((spirit = cJSON_CreateObject()) == NULL) goto end;
    if ((red = cJSON_CreateArray()) == NULL) goto end;
    if ((blue = cJSON_CreateArray()) == NULL) goto end;
    if ((ball_spirit = cJSON_CreateObject()) == NULL) goto end;

    cJSON_AddItemToObject(spirit, "red", red);
    cJSON_AddItemToObject(spirit, "blue", blue);
    cJSON_AddItemToObject(spirit, "ball", ball_spirit);

    for (int i = 0; i < MAX_TEAM; i++) {
        if (red_team[i].online == 1) {
            if (add_player_to_team(red, &red_team[i]) < 0) goto end;          
        }
        if (blue_team[i].online == 1) {
            if (add_player_to_team(blue, &blue_team[i]) < 0) goto end;      
        }          
    }
    char tmp[512] = {0};
    sprintf(tmp, "%s", ball_status.by_team ? "blue" : "red");
    if ((who = cJSON_CreateString(tmp)) == NULL) goto end;
    if ((name = cJSON_CreateString(ball_status.name)) == NULL) goto end;
    if ((x = cJSON_CreateNumber((int)ball.x)) == NULL) goto end;
    if ((y = cJSON_CreateNumber((int)ball.y)) == NULL) goto end;

    cJSON_AddItemToObject(ball_spirit, "who", who);
    cJSON_AddItemToObject(ball_spirit, "name", name);
    cJSON_AddItemToObject(ball_spirit, "x", x);
    cJSON_AddItemToObject(ball_spirit, "y", y);

    sprintf(tmp, "ball.x = %d, ball.y = %d", (int)ball.x, (int)ball.y);
    //show_message(NULL, NULL, tmp, 1);
    string = cJSON_Print(spirit);
end:
    cJSON_Delete(spirit);
    return string;
}
