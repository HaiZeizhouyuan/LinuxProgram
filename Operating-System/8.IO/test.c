/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Sat Aug  8 15:02:55 2020
 ************************************************************************/

#include "head.h"
int main () {
    char *tmp;
    if ((tmp = get_conf("./test.conf", "MY_NAME")) == NULL) {
        fprintf(stderr, "Error in conf!\n");
        exit(1);
    }
    printf("MY_NAME : %s\n", conf_value_ans);
    printf("%s\n", tmp);
    make_non_block(0);
    char buffer[1024] = {0};
    scanf("%s", buffer);
    printf("%s\n", buffer);
    make_block(0);
    printf("%s\n", buffer);
    return 0;
}

