
/*************************************************************************
	> File Name: ls2.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Jul 25 19:11:02 2020
 ************************************************************************/
#include "head.h"

#define MAXNAME 512
#define MAXFILE 1024

void do_ls(char *);
void show_info(char *, struct stat *);
void output(char *, struct stat *);
int l_flag = 0, a_flag = 0;

char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t name);

int bg_c, fg_c;

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
    DBG(GREEN"<Debug>"NONE": optind = %d\n", optind);
    DBG(GREEN"<Debug>"NONE" : l = %d, a = %d\n", l_flag, a_flag);
    DBG(GREEN"<Debug>"NONE" : argv[optind] = %s\n", argv[optind]);
    DBG(GREEN"<Debug>"NONE" : argv[optind - 1] = %s\n", argv[optind - 1]);
    DBG(GREEN"<Debug>"NONE" : argv[0] = %s\n", argv[0]);
    argc -= optind;
    argv += optind;

    if (argc == 0) {
        do_ls(".");
        DBG(GREEN"<Debug>"NONE": Doing with .\n");
    } else {
        while(argc--){
            do_ls(*(argv++));
            DBG(GREEN"<Debug>"NONE" : Doing with %s\n", *(argv - 1));
        }
    }
    //do_ls(".");
    return 0;
}

int cmp_name(const void* _a, const void* _b) {
    char *a = (char *)_a;
    char *b = (char *)_b;
    return strcmp(a, b);
}

void update_color(mode_t mode) {
    bg_c = 0;
    fg_c = 31;
    if (mode & (S_IXGRP | S_IXUSR | S_IXOTH)) {
        fg_c = 32;
    } //或if (mode & 0111);
    switch(mode & S_IFMT) {
        case S_IFDIR:
            fg_c = 34;
            break;
        case S_IFLNK:
            fg_c = 35;
            break;
    }
    return ;
}

void size_window(char names[][MAXNAME], int cnt, int *row, int *col) {
    struct winsize size;
    int len[cnt], max = 0, total_len = 0;
    memset(len, 0, sizeof(int) * cnt);
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl()");
        exit(1);
    }
    DBG(GREEN"<Debug>"NONE" : win.row = %d, win.col = %d\n", size.ws_row, size.ws_col);
    for (int i = 0; i < cnt; i++) {
        len[i] = strlen(names[i]);
        if (max < len[i]) max = len[i];
        total_len += len[i] + 1;
    }
    if (max + 1 >= size.ws_col) {
        *row = cnt;
        *col = 1;
        return ;
    }
    if (total_len <= size.ws_col) {
        *row = 1;
        *col = cnt;
        printf("zzzzzzzzzzzzz ");
        return ;
    }
    int try_begin = 0;
    for (int i = 0, tmp = 0; i < cnt; i++) {
        tmp += (len[i] + 1);//此时的长度
        if (tmp >= size.ws_col) {
            try_begin = i;
            break;
        }
    }
    for (int i = try_begin; ; i--) {
        int *wide = (int *)malloc(sizeof(int) * i);//每列的宽度
        memset(wide, 0, sizeof(int) * i);
        *row = (int)ceil(cnt * 1.0 / i);//上取整
        int sum = 0;
        for (int x = 0; x < i; x++) {//x是列
            for (int y = x * *row; y < (x + 1) * *row && y < cnt; y++) {
                if (wide[x] < len[y]) wide[x] = len[y];
            }
            sum += (wide[x] + 1);
        }
        if (sum > size.ws_col) continue;
        if (sum <= size.ws_col) {
            *col = i;
            break;
        }
    }
}

void show_files(char names[][MAXNAME], int cnt, int row, int col) {
    int wide[col];
    memset(wide, 0, sizeof(int) * col);
    struct stat tmp_st;
    for (int i = 0; i < col; i++) {
        for (int j = (i * row); j < (i + 1) * row && j < cnt; j++) {
            if (wide[i] < strlen(names[j])) wide[i] = strlen(names[j]);
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = i; (j <= i + (row * col)) && j < cnt; j = j + row) {
            lstat(names[j], &tmp_st);
            update_color(tmp_st.st_mode);
            int wide_tmp = wide[j / row];
            printf("\033[%d;%dm%-*s \033[0m", bg_c, fg_c, wide_tmp, names[j]);
        }
        printf("\n");
    }
}

void do_ls(char *dir) {
    DIR *dirp = NULL;
    struct dirent *direntp;
    DBG(GREEN"<Debug>"NONE ": %s\n", dir);
    if ((dirp = opendir(dir)) == NULL) {//目录打开失败
        if (access(dir, R_OK) == 0) {//判断文件是否可读
            if (l_flag == 1) {
                struct stat tmp_st;
                lstat(dir, &tmp_st);
                show_info(dir, &tmp_st);
                printf("\n\n");
            } else {
                printf("%s\n", dir);
            }
        } else {
            perror(dir);
            exit(1);
        }
    } else {//是目录遍历目录
        char names[MAXFILE][MAXNAME] = {0};
        printf("%s :\n", dir);
        int cnt = 0;
        while ((direntp = readdir(dirp)) != NULL) {
            if (direntp->d_name[0] == '.' && (a_flag == 0)) continue;
            strcpy(names[cnt++], direntp->d_name);
        }
        qsort(names, cnt, MAXNAME, cmp_name);
        chdir(dir);//跳转目录，改变路径
        if (l_flag == 1) {
            for (int i = 0; i < cnt; i++) {
                struct stat st_tmp;
                lstat(names[i], &st_tmp);
                show_info(names[i], &st_tmp);
                printf("\n");
            }
        } else {
            int row, col;
            size_window(names, cnt, &row, &col);
            DBG(GREEN"<Dbuge>"NONE" : row = %d, col = %d\n", row, col);
            show_files(names, cnt, row, col);
        }
    }
    return ;
}


char *uid_to_name(uid_t uid) {
    struct passwd *pw_ptr;
    static char tmpstr[10] = {0};
    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(tmpstr, "%d", uid);
        return tmpstr;
    } else {
        return pw_ptr->pw_name;
    }
}

char *gid_to_name(gid_t gid) {
    struct group *gr_ptr;
    static char tmpstr[10] = {0};
    if ((gr_ptr = getgrgid(gid)) == NULL) {
        sprintf(tmpstr, "%d", gid);
        return tmpstr;
    } else {
        return gr_ptr->gr_name;
    }
}

void mode_to_str(mode_t mode, char *modestr) {

    strcpy(modestr, "----------");
    if (S_ISREG(mode)) modestr[0] = '-';
    if (S_ISDIR(mode)) modestr[0] = 'd';
    if (S_ISCHR(mode)) modestr[0] = 'c';
    if (S_ISBLK(mode)) modestr[0] = 'b';
    if (S_ISSOCK(mode)) modestr[0] = 's';
    if (S_ISFIFO(mode)) modestr[0] = 'p';
    if (S_ISLNK(mode)) modestr[0] = 'l';

    if (S_IRUSR & mode) modestr[1] = 'r';
    if (S_IWUSR & mode) modestr[2] = 'w';
    if (S_IXUSR & mode) modestr[3] = 'x';

    if (S_IRGRP & mode) modestr[4] = 'r';
    if (S_IWGRP & mode) modestr[5] = 'w';
    if (S_IXGRP & mode) modestr[6] = 'x';
    
    if (S_IROTH & mode) modestr[7] = 'r';
    if (S_IWOTH & mode) modestr[8] = 'w';
    if (S_IXOTH & mode) modestr[9] = 'x';
    update_color(mode);
    printf("%d\n", fg_c);
}


void show_info (char *filename, struct stat *st) {
    char modestr[15] = {0};
    mode_to_str(st->st_mode, modestr);
    DBG("1\n");
    printf("%s ", modestr);
    printf("%4ld ", st->st_nlink);
    printf("%10s ", uid_to_name(st->st_uid));
    printf("%10s ", gid_to_name(st->st_gid));
    printf("%10ld ", st->st_size);
    printf("%.15s ", 4 + ctime(&st->st_mtime));
    printf("\033[%d;%dm%s\033[0m ",bg_c, fg_c, filename);
}

