/*************************************************************************
	> File Name: parse_spirit.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月01日 星期日 19时38分02秒
 ************************************************************************/

#include "head.h"

void parse_spirit(char *msg, int size) {
    cJSON *conf = NULL;
    if ((conf = cJSON_Parse(msg)) == NULL) {
        const char *err_ptr = cJSON_GetErrorPtr();
        if (err_ptr != NULL) {
            fprintf(stderr, "Error Before : %s\n", err_ptr);
            return ;
        }
    }
    get_map_value(conf);
    return ;
}

int get_json_valueint(cJSON *conf, const char *who, const char *config) {
    const cJSON *user = NULL;
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_port = cJSON_GetObjectItemCaseSensitive(user, config);
end :
    return user_port->valuedouble;
}

char *get_json_valuestring(cJSON *conf, const char *who, const char *config) {
    cJSON *user = NULL;
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_desc = cJSON_GetObjectItemCaseSensitive(user, config);
end :
    return user_desc->valuestring;
}



