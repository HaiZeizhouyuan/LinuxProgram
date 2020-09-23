/*************************************************************************
	> File Name: test1.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月23日 星期三 15时29分13秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
 
char filename[10], recv_name[10];

int main() {
    scanf("%s", filename);
    //scanf("%s", recv_name);
    printf("filename : %s!\n", filename);
    printf("%ld\n", strlen(filename));
    return 0;
}
