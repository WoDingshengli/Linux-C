/*仿写sleep函数*/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_alrm(int);
unsigned int mysleep(unsigned int);

int main()
{
    while(1){
        mysleep(2);
        printf("Two seconds passed\n");
    }

    return 0;
}

unsigned int mysleep(unsigned int secs)
{
    struct sigaction newact, oldact;
    unsigned int unslept;

    newact.sa_handler = sig_alrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM , &newact, &oldact);

    alarm(secs);
    pause();

    unslept = alarm(0);
    sigaction(SIGALRM , &oldact, NULL);

    return unslept;
}

void sig_alrm(int signo)
{
    printf("Hello world");
}
