#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    char *message;
    int n;
    pid = fork();
    if(pid < 0) perror("fork failed!");
    if(pid == 0)
    {
        message = "Child\n";
        n = 8;
    }else{
        message = "Parent\n";
        n = 4;
    }
    for(; n > 0; n --){
        printf("%s",message);
        printf("%d\n",getpid());
        sleep(1);
    }

    return 0;
}

