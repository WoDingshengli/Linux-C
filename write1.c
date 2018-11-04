/*
 *给另一个终端发送消息
 */

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define BUFSIZE 512

int main(int ac, char *av[])
{
    int fd; //另一个终端到文件描述符
    char buf[BUFSIZE];//缓存消息
    if(ac != 2){
        fprintf(stderr, "write1");
        exit(1);
    }

    fd = open(av[1], O_WRONLY);
    if(fd == 1){
        perror(av[1]);
        exit(1);
    }

    while(fgets(buf, BUFSIZE, stdin) != NULL)
        if(write(fd, buf, strlen(buf)) == -1)
            break;
    close(fd);
    
    return 0;
}

