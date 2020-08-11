/*************************************************************************
	> File Name: scandir.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: Tue Aug 11 15:29:01 2020
 ************************************************************************/

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int sdlect(const struct dirent *);

int main(void) {
    struct dirent **namelist;
    int n;
	n = scandir(".", &namelist, sdlect, alphasort);
    if (n == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    while (n--) {
        printf("%s\n", namelist[n]->d_name);
        free(namelist[n]);
    }
    free(namelist);
	exit(EXIT_SUCCESS);
    return 0;
}
static int sdlect(const struct dirent* dirp) {   
    printf("****dirname = %s\n", dirp->d_name);   
    if  (!strcmp(dirp->d_name,"."))   
        return 0;   
    else if (!strcmp(dirp->d_name,".."))   
        return   0;   
    return  1;   
} 
