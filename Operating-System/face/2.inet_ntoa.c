/*************************************************************************
	> File Name: 2.inet_ntoa.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月16日 星期三 19时23分27秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<inttypes.h>
//192.168.0.1
char *my_inet_ntoa(struct in_addr in) {
    static char ip[20] = {0};
    uint8_t *p = (char *)&in.s_addr;
    sprintf(ip, "%d.%d.%d.%d\n",p[0], p[1], p[2], p[3]);
    return ip;
}

int main() {
    char input_ip[20] = {0};
    scanf("%s", input_ip);
    printf("input_ip = %s\n", input_ip);
    printf("uint32_t ip = %d\n", inet_addr(input_ip));
    struct in_addr in;
    in.s_addr = inet_addr(input_ip);
    printf("Ip After change : %s\n", my_inet_ntoa(in));
    return 0;
}
