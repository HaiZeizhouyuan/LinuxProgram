/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Tue Aug  4 15:26:26 2020
 ************************************************************************/

#include "head.h"
#include "m_popen.h"

int main () {
    FILE * fp;
    char buff[1024] = {0};
    if ((fp = m_popen("ls /etc", "r")) == NULL) {
        perror("m_popen()");
        exit(1);
    }

    while(fgets(buff, sizeof(buff), fp) != NULL){
        printf("%s", buff);
        bzero(buff, sizeof(buff));
    }
    int status = m_pclose(fp);
    printf("status : %d\n", status);
    return 0;
}
