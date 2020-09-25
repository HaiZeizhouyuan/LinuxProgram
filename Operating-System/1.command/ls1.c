/*************************************************************************
	> File Name: ls.c
	> Author: 
	> Mail: 
	> Created Time: Sat Jul 25 19:11:02 2020
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
void do_ls(char *);
void show_info(char *, struct stat *);
void output(char *, struct stat *);
int  l_flag = 0, a_flag = 0;

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt) {
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
    return 0;
}


void do_ls(char *dir) {
    DIR *dirp = NULL;
    struct dirent *direntp;
    if ((dirp = opendir(dir)) == NULL) {
        exit(1);
    }
    //while ((direntp = readdir(dirp)) != NULL) {
         
   // } 
    struct dirent **namelist;
    int cout;
    cout = scandir(".", &namelist, 0, alphasort);
    if (cout < 0) {
        printf("scandir");
    } else {
        int i = 0;
        while (i < cout) {
            struct stat st;
            if ((a_flag == 0) && (namelist[i]->d_name[0] == '.')) {
                i += 1;
                continue;
            }
            if (l_flag == 0) {
                if (lstat(namelist[i]->d_name, &st) < 0) {
                    perror("lstat");
                    free(namelist[i]);
                    i += 1;
                    exit(1);
                } else {
                    output(namelist[i]->d_name, &st);
                    i += 1;
                }
            } else {
                if (lstat(namelist[i]->d_name, &st) < 0) {
                    perror("lstat");
                    free(namelist[i]);
                    i += 1;
                    exit(1);
                } else {
                    show_info(namelist[i]->d_name, &st);
                    free(namelist[i]);
                    i += 1;
                }
            }
        }
        free(namelist);
    }
    printf("\n");
}
void output(char *filename,struct stat *st) {
    if (S_IXUSR & st->st_mode) {
        if (S_ISDIR(st->st_mode)) printf("\033[34m%s \033[0m", filename);
        else printf("\033[32m%s \033[0m", filename);
    } else {
        printf("%s ", filename);
    }
    return ;
}

char *substring (char* ch, int pos, int length) {
    char *newch = (char *)calloc(sizeof(char), (length + 1));
    int j = 0;
    for (int i = pos; i < length; i++) {
        newch[j++] = ch[i];
    } 
    int len2 = strlen(newch);
    newch[len2 - 1] = '\0';
    return newch;
}

int dight (int n) {
    if (n == 0) return 1;
    return floor(log10(n)) + 1;
}

void printblank(int num) {
    int lennum = dight(num);
    for (int i = 0; i < 6 - lennum; i++) {
        printf(" ");
    }
    return ;
}

void show_info(char *filename, struct stat *st) {
    struct group *gr;
    struct tm *timer;
    char ch[25];
    //char **modestr = (char **)calloc(15, sizeof(char*));
    char modestr[15] = {0};
    char *nowtime;
    struct passwd *p = getpwuid(getuid());
    struct group *q = getgrgid(getgid());
    strcpy(modestr, "----------");
    sprintf(ch, "%s",ctime(&st->st_mtime));
    int len = strlen(ch);
    nowtime = substring (ch, 4, len - 8);
    
    if (S_ISDIR(st->st_mode)) modestr[0] = 'd';
    if (S_IRUSR & st->st_mode) modestr[1] = 'r';
    if (S_IWUSR & st->st_mode) modestr[2] = 'w';
    if (S_IXUSR & st->st_mode) modestr[3] = 'x';

    if (S_IRGRP & st->st_mode) modestr[4] = 'r';
    if (S_IWGRP & st->st_mode) modestr[5] = 'w';
    if (S_IXGRP & st->st_mode) modestr[6] = 'x';

    if (S_IROTH & st->st_mode) modestr[7] = 'r';
    if (S_IWOTH & st->st_mode) modestr[8] = 'w';
    if (S_IXOTH & st->st_mode) modestr[9] = 'x';
    if (S_IXUSR & st->st_mode) {
        if (modestr[0] == 'd') {
            printf("%s  %d", modestr, st->st_nlink);
            printblank(st->st_nlink);
            printf("%s  %s  %d  ", p->pw_name, q->gr_name, st->st_size);
            printblank (st->st_size);
            printf("%s  \033[34m%s\033[0m\n", nowtime, filename);
        } else { 
            printf("%s  %d", modestr, st->st_nlink);
            printblank(st->st_nlink);
            printf("%s  %s  %d  ", p->pw_name, q->gr_name, st->st_size);
            printblank (st->st_size);
            printf("%s  \033[32m%s\033[0m\n",nowtime, filename);
        }
    } else {
        printf("%s  %d", modestr, st->st_nlink);
        printblank(st->st_nlink);
        printf("%s  %s  %d  ", p->pw_name, q->gr_name, st->st_size);
        printblank (st->st_size);
        printf("%s  %s\n",nowtime, filename);
    }
    return ;
}  
