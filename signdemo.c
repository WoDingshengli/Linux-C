/*信号捕捉和处理*/
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void f(int );

int main()
{
    int i;
    signal(SIGINT, f);
    for(i = 0; i < 5; i++)
    {
        printf("hello world!\n");
        sleep(1);
    }
    return 0;
}

void f(int signum)
{
    printf("signal\n");
}
