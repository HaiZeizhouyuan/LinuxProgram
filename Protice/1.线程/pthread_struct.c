/*************************************************************************
	> File Name: pthread_struct.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月24日 星期六 14时54分58秒
 ************************************************************************/

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct Stu {
    int age;
    char name[20];
};
struct Stu stu;
void *func(void *arg) {
    struct Stu *temp = (struct Stu *)arg;
    printf("I am %s , %d old\n", temp->name, temp->age);
}

int main() {
    pthread_t tid;
    stu.age = 24;
    strcpy(stu.name, "zhouyaun");
    pthread_create(&tid, NULL, func, (void *)&stu);
 //   sleep(1);
    pthread_join(tid, NULL);
    return 0;
}
