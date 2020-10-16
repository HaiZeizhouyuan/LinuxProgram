/*************************************************************************
	> File Name: static.c
	> Author: zhouyuan
	> Mail: 
	> Created Time: 2020年08月15日 星期六 11时33分38秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void func() {    
    static int x = 0; // 在对func的三次调用中,x只进行一次初始化    
    printf("%d\n", x); // 输出x的值   
    x = x + 1;
}

int main(int argc, char * const argv[]) {    
    func(); // 输出0    
    func(); // 输出1    
    func(); // 输出2 
    return 0;
}

