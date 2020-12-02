/*************************************************************************
	> File Name: parse_spirit.h
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年11月01日 星期日 19时37分36秒
 ************************************************************************/

#ifndef _PARSE_SPIRIT_H
#define _PARSE_SPIRIT_H
void parse_spirit(char *msg, int size );
cJSON *get_cjson(const char *file);
int get_json_valueint(cJSON *conf, const char *who, const char *config);
char *get_json_valuestring(cJSON *conf, const char *who, const char *config);

#endif
