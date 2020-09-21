/*************************************************************************
	> File Name: fwrite.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月17日 星期四 18时59分33秒
 ************************************************************************/

#include "head.h"

int main() {
    FILE *fp1;
    FILE *fp2;
    int fw;
    char filename[100] = {"fwrite.txt"};
    char tmpfile[100];
    fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
        strcpy(tmpfile, filename);
    } else {
        printf(BLUE"%s have!\n"NONE, filename);
        int ans = 1;
        while(1) {
            sprintf(tmpfile, "%s%d", filename, ans);
            if ((fp1 = fopen(tmpfile, "r")) == NULL) {
                break;
            }
            memset(tmpfile, 0, sizeof(tmpfile));
            ans += 1;
        }
    }
    printf(RED"create new %s\n"NONE, tmpfile); 
        fp2 = fopen(tmpfile, "w");
        char buff[] = "zhouyuan"; 
        if ((fw = fwrite(buff, sizeof(buff), 1,fp2)) < 0) {
            perror("fwrite()");
            exit(0);
        }
    return 0;
}
