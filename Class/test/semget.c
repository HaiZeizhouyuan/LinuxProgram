#include<stdio.h>
#include<sys/sem.h>
#define MYKEY 6666
int main() { 
    int semid;
    semid=semget(MYKEY,1,IPC_CREAT|0666);//创建了一个权限为666的信号量
    printf("semid=%d\n",semid);
    return 0;
}
