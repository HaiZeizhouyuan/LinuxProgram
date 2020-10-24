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
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_port = cJSON_GetObjectItemCaseSensitive(user, config);
    
end :
    //cJSON_Delete(conf);
    return user_port->valueint;
}

char *get_json_valuestring(cJSON *conf, const char *who, const char *config) {
    cJSON *user = NULL;
    user = cJSON_GetObjectItemCaseSensitive(conf, who);
    cJSON *user_desc = cJSON_GetObjectItemCaseSensitive(user, config);
end :
    //cJSON_Delete(conf);
    return user_desc->valuestring;
}

int get_timer(cJSON *slave, const char *info) {
    cJSON *sla = NULL;
  //  printf("%s\n", info);
    sla = cJSON_GetObjectItemCaseSensitive(slave, "Slave");
    cJSON *sla_time = cJSON_GetObjectItemCaseSensitive(sla, "Timer");
    cJSON *in = cJSON_GetObjectItemCaseSensitive(sla_time, info);
    DBG(BLUE"get slave info success...\n"NONE);
end :
    return in->valueint;
}

