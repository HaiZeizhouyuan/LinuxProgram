/*************************************************************************
	> File Name: parse_conf.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月18日 星期日 15时29分02秒
 ************************************************************************/

#include "head.h"


cJSON *get_cjson(const char *file) {
    FILE *f = NULL;
    char *buff = (char *)calloc(512 , sizeof(char));
    if ((f = fopen(file, "rb")) == NULL) {
        perror("fopen()");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buff, len, 1, f);
    if (len > strlen(buff)) {
        fprintf(stderr, "Buffer is less than the len of file!\n");
        return NULL;
    }
    const cJSON *master = NULL;
    const cJSON *slave = NULL;
    cJSON *conf = NULL;
    if ((conf = cJSON_Parse(buff)) == NULL) {
        const char *err_ptr = cJSON_GetErrorPtr();
        if (err_ptr != NULL) {
            fprintf(stderr, "Error Before : %s\n", err_ptr);
            return NULL;
        }
    }
    //printf("%s\n", buff);
    fclose(f);
    return conf;
}

int get_json_valueint(cJSON *conf, const char *who, const char *config) {
    const cJSON *user = NULL;
    int val = -1;
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_port = cJSON_GetObjectItemCaseSensitive(user, config);
    val = user_port->valueint;
    
end :
    //cJSON_Delete(conf);
    return val;
}

char *get_json_valuestring(cJSON *conf, const char *who, const char *config) {
    cJSON *user = NULL;
    char *str = (char *)calloc(512, sizeof(char));
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_desc = cJSON_GetObjectItemCaseSensitive(user, config);
    strcpy(str, user_desc->valuestring);
end :
    //cJSON_Delete(conf);
    return str;
}


