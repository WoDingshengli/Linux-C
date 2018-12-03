/*输出重定向*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int  fd;
    int  newfd;
    char line[100];
    
    int i = 3;
    while(i > 0){
      fgets(line , 100, stdin);printf("%s",line);
      i --;
    }

    fd = open("data", O_RDONLY);
   #ifdef CLOSE_DUP
    close(0);
    newfd = dup(fd);
   #else
    newfd = dup2(fd ,0);
   #endif
    if(newfd != 0){
        fprintf(stderr , "Could not duplicate fd to 0\n");
        exit(1);
    }

    close(fd);

    i = 3;
    while(i > 0){
      fgets(line , 100, stdin);printf("%s", line);
      i --;
    }

    return 0;
}

    
