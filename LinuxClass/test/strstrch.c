/*************************************************************************
	> File Name: strstrch.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月19日 星期六 11时32分24秒
 ************************************************************************/

#include <string.h>
#include <stdio.h>

void main()
{
        char * pCh = "www.inkcool.com";
        char * pFind = strrchr(pCh, '.');
        if ( pFind != NULL )
    {
                printf("%s\n", pFind);    //可以直接printf(pFind);printf("/n");左边的表达式是合二为一的表达方法；
            
    }

}
