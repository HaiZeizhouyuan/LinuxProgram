/*************************************************************************
	> File Name: parse_conf.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月18日 星期日 15时28分18秒
 ************************************************************************/

#ifndef _PARSE_CONF_H
#define _PARSE_CONF_H
cJSON *get_cjson(const char *file);
int get_json_valueint(cJSON *conf, const char *who, const char *config);
char *get_json_valuestring(cJSON *conf, const char *who, const char *config);


//int parse_conf(char *string);
//int get_string(const char *file, char *buff, size_t size);
#endif
