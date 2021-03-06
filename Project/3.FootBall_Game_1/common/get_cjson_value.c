/*************************************************************************
	> File Name: get_cjson_value.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月21日 星期三 23时09分51秒
 ************************************************************************/

#include "head.h"

char *get_cjson_value(const char *path, const char *key) {
    FILE *f = NULL;
    char *buff = (char *)calloc(512 , sizeof(char));
    if ((f = fopen(path, "rb")) == NULL) {
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
    const cJSON *foot_info = NULL;
    foot_info = cJSON_GetObjectItemCaseSensitive(conf, key);
    return foot_info->valuestring;
}

