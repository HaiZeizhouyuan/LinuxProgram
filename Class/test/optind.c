/*************************************************************************
	> File Name: optind.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 18时40分07秒
 ************************************************************************/

#include "../common/head.h"

int main(int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "p:h:n:m:")) != -1) {
        switch(opt) {
            case 'p':
                break;
            case 'h':
                break;
            case 'n':
                break;
            case 'm':
                break;
            default :
                fprintf(stderr, "Usge : %s [-phnm]!\n", argv[0]);
                exit(1);
        }
        printf("argc : %d , optind : %d\n", argc, optind);
    }
    printf("optind : %d\n", optind);
    return 0;
}
