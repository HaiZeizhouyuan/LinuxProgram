/*************************************************************************
	> File Name: ls.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Jul 25 16:01:36 2020
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void do_ls(char *);
void show_info(char *, struct stat *);
int  l_flag = 0, a_flag = 0;
int main(int argc, char **argv) {
    int opt, l_flag = 0, a_flag = 0;
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch(opt) {
            case 'l':
                l_flag = 1;
                break;
            case 'a':
                a_flag = 1;
                break;
            default:
            fprintf(stderr, "Usage : %s [-la]!\n", argv[0]);
                exit(1);
        }
    }
    do_ls(".");
}
void do_ls(char *dir) {
    DIR *dirp = NULL;
    struct dirent *direntp;
    if ((dirp = opendir(dir)) == NULL) {
        exit(1);
    }
    while ((direntp = readdir(dirp)) != NULL) {
        if ((a_flag == 0) && (direntp->d_name[0] == '.')) continue;
        if (l_flag == 0) {
            printf("%s ", direntp->d_name);
        } else {
            struct stat st;
            if (lstat(direntp->d_name, &st) < 0) {
                perror("lstat");
                exit(1);
            } else {
                show_info(direntp->d_name, &st);
            }
        }
    }
    printf("\n");
}


void show_info(char *filename, struct stat *st) {
    char modestr[15] = {0};
    strcpy(modestr, "----------");
    if (S_ISDIR(st->st_mode)) modestr[0] = 'd';
    if (S_IRUSR & st->st_mode) modestr[1] = 'r';
    if (S_IWUSR & st->st_mode) modestr[2] = 'w';
    if (S_IXUSR & st->st_mode) modestr[3] = 'x';
    if (S_IXUSR & st->st_mode)
        printf("%s %d %d %d   \033[32m%s\033[0m\n", modestr, st->st_uid, st->st_gid, st->st_size, filename);
    else
        printf("%s %d %d %d   %s\n", modestr, st->st_uid, st->st_gid, st->st_size, filename);
}
