/*************************************************************************
	> File Name: 2.popen.c
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 14:16:59 2020
 ************************************************************************/

#include "head.h"

int main() {
    FILE *pr, *fw;
    char buffer[1024];
    fw = fopen("popen_r.txt", "w");
    pr = popen("ls /etc", "r");

    if (pr == NULL) {
        perror("pr NULL");
        exit(1);
    }
    
    size_t fr;
    while ((fr = fread(buffer, sizeof(buffer), 1, pr)) >= 0) {
        //printf("%s\n", buffer);
        fwrite(buffer,  sizeof(buffer), 1, fw);
        memset(buffer, 0, strlen(buffer));
        if (feof(pr)) break;
    }
    pclose(pr);
    return 0;
}
