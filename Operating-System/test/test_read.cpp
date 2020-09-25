/*************************************************************************
	> File Name: test_read.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  2 10:32:10 2020
 ************************************************************************/

#include "head.h"

typedef struct infor {
    int now, sum;
} infor;

int main () {
    int fd = open("name.file", O_RDWR|O_CREAT, 0666);
	infor inf;
	memset((void*)&inf, 0x00, sizeof(infor));
	inf.now = 0;
	inf.sum = 0;
    write(fd, (void *)&inf, sizeof(infor));
    printf("qnow: %d, qsum: %d\n", inf.now, inf.sum);
    for (int i = 0; i < 5; i++) {
        read(fd, (void*)&inf, sizeof(infor));
        printf("%d old  now is : %d, and sum is : %d\n", i, inf.now, inf.sum);
        int sum1 = inf.sum, now1 = inf.now;
        sum1 += 5;
        now1 += 5;
        memset((void*)&inf, 0x00, sizeof(infor));
        inf.now = now1;
        inf.sum = sum1;
        write(fd, (void*)&inf, sizeof(infor));
        printf("%d new now is : %d, and sum is :%d\n", i, inf.now, inf.sum);
    }
    write(fd, NULL, sizeof(infor));
    close(fd);
    return 0;
}
