/*************************************************************************
	> File Name: cat.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Jul 28 14:46:15 2020
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void read_file (char *, char *);

int n = 1;

int main (int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage : %s file...\n", argv[0]);
        exit(1);
    }
    int i = 1;
    if (argv[1][1] == 'n') i = 2;
    while (i <= argc - 1) {
        //printf("%s\n", argv[i]);
        read_file(argv[i], argv[1]);
        i += 1;
    }
    return 0;
}

void read_file(char *pathname, char *g1) {
    FILE *file = NULL;
    char buff[1024] = {0};
    if ((file = fopen(pathname, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    while (fgets(buff, sizeof(buff), file) != NULL) {
        if (g1[1] == 'n') printf("%d %s", n, buff);
        else printf("%s", buff);
        n += 1;
        bzero(buff, sizeof(buff));
    }
}

