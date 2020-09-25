/*************************************************************************
	> File Name: cat.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Jul 28 14:46:15 2020
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void read_file (char *, int , int );
int n = 1,flag_n = 0, flag_b = 0;
int main (int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "n:: b::")) != -1) {
        switch(opt) {
            case 'n':
                flag_n = 1;
                break;
            case 'b':
                flag_b = 1;
                break;
            default :
                fprintf(stderr, "Usage : %s -n -b !\n", argv[0]);
        }
    }
    if (argc == 1) {
        fprintf(stderr, "Usage : %s file...\n", argv[0]);
        exit(1);
    }
    int i = 1;
    if (argv[1][1] == 'n') i = 2;
    if (argv[1][1] == 'b') i = 2;
    if (argv[1][1] == 'n' && argv[2][1] == 'b') i = 3;
    while (i <= argc - 1) {
        //printf("%s\n", argv[i]);
        read_file(argv[i], flag_n, flag_b);
        i += 1;
    }
    return 0;
}

void read_file(char *pathname, int flag_n, int flag_b) {
    FILE *file = NULL;
    char buff[1024] = {0};
    if ((file = fopen(pathname, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    while (fgets(buff, sizeof(buff), file) != NULL) {
        int a = strcmp(buff, "\n");
        if (flag_n && !flag_b) {//-n
            printf("%d %s", n++, buff);
        } else if ((flag_n && flag_b) || (!flag_n && flag_b)) {//-n -b || -b
            if (a) printf("%d %s", n++, buff);
            else printf("%s", buff);
        } else  {
            printf("%s", buff);
        }
        bzero(buff, sizeof(buff));
    }
}

