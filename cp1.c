/*cp的第一个版本
 *流程是：
 *1.打开原文件和要拷贝到的文件
 *2.将原文件中到数据读到缓存区，再从缓存区读到拷贝到的文件中
 *3.关闭两个文件
 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

#define BUFFLEN 4096
#define COPYMODE 0644 /*-rw-r--r--*/

void oops(char *, char *);

int main(int argc, char *argv[])
{
    int in_fd, out_fd, n_chars;    
    char buf[BUFFLEN];
    if(argc != 3){
        fprintf(stderr, "check your arguments please!\n");
        exit(1);
    }               /*check arguments*/

    if((in_fd = open(argv[1], O_RDONLY)) == -1)
        oops("Cannot open", argv[1]); /*open file*/

    if((out_fd = creat( argv[2], COPYMODE)) == -1)
        oops("Cannot creat", argv[2]);    /*creat copyfile*/

    while((n_chars = read(in_fd, buf, BUFFLEN)) > 0)
        if( write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to", argv[2]);  /* write*/

    if(n_chars == -1)
        oops("Read error from", argv[1]);

    if( close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");

    return 0;
}


void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error:%s", s1);
    perror(s2);
    exit(1);
}



