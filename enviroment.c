/*环境变量的使用*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char *cp = getenv("LANG");
    if(cp != NULL && strcmp(cp , "zh_CN.UTF-8") == 0)
        printf("你好\n");
    else
        printf("Hello\n");

    return 0;
}
