/*学习管道机制*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 100

int main()
{
    int n;
    int fd[2]; /*fd[0]指向管道的读端*/
    pid_t pid;
    char line[MAXLINE];

    if(pipe(fd) < 0){perror("pipe"); exit(1);}
    if((pid = fork()) < 0){perror("fork"); exit(1);}
    if(pid > 0){
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
        wait(NULL);/*使父进程阻塞，等待子进程终止信息*/
    } else {
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    return 0;
}
