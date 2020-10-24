/*************************************************************************
	> File Name: pthread1.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年10月19日 星期一 21时57分57秒
 ************************************************************************/

#include "head.h"
 
  
void *PrintHello(void *args){
    int thread_arg;
    sleep(1);
    thread_arg = (int)(*((int*)args));
    printf("Hello from thread %d\n", thread_arg);
    return NULL;

}
   
int main() {
    int rc,t;
    pthread_t thread[10];      
    for( t = 0; t < 8; t++ ) {
        printf("Creating thread %d\n", t);
        //此处t变量的用法是方便大家调测代码的写法，实际使用会有问题，因为这个t是局部变量，
        //函数执行完后马上释放，大家传递参数时需要使用全局变量或malloc出来的变量。    
        rc = pthread_create(&thread[t], NULL, PrintHello, &t);
        if (rc) {
            printf("ERROR; return code is %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    sleep(5);
    for( t = 0; t < 8; t++ ) {
        pthread_join(thread[t], NULL);
        return EXIT_SUCCESS;
    }
    return 0;
}
