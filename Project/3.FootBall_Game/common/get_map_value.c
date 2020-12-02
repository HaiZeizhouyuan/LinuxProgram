/*************************************************************************
	> File Name: get_map_value.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 23时09分51秒
 ************************************************************************/

#include "head.h"
extern struct BallStatus ball_status;
extern struct Bpoint ball;
extern struct Score score;
extern struct User *blue_team, *red_team;
const cJSON *red = NULL;
const cJSON *blue = NULL;
extern int red_num, blue_num;
void get_map_value(cJSON *conf) {
    ball.x =  get_json_valueint(conf, "ball", "x");
    ball.y =  get_json_valueint(conf, "ball", "y");
    ball_status.by_team = get_json_valueint(conf, "ball", "who");
    strcpy(ball_status.name, get_json_valuestring(conf, "ball", "name"));
    score.red = get_json_valueint(conf, "score", "red");
    score.blue = get_json_valueint(conf, "score", "blue");
    get_user(conf, "red");
    get_user(conf, "blue");
    
}

void get_user(cJSON *conf, const char *team) { 
    int sum = 0;
    cJSON *teams  = NULL;
    cJSON *tmp_team = cJSON_GetObjectItemCaseSensitive(conf, team);
    cJSON_ArrayForEach(teams, tmp_team) {
        cJSON *x = cJSON_GetObjectItemCaseSensitive(teams, "x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(teams, "y");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(teams, "name");
        if (!cJSON_IsNumber(x) || !cJSON_IsNumber(y) || !cJSON_IsString(name)) {
            return ;
        }
        if (strcmp(team, "red") == 0) {
            red_team[sum].loc.x = x->valueint;
            red_team[sum].loc.y = y->valueint;
            strcpy(red_team[sum].name, name->valuestring);
        } else {
            blue_team[sum].loc.x = x->valueint;
            blue_team[sum].loc.y = y->valueint;
            strcpy(blue_team[sum].name, name->valuestring);
        }
        sum += 1;
    }
    if (strcmp(team, "red") == 0) {
        red_num = sum;
    } else {
        blue_num = sum;
    }
    
}


