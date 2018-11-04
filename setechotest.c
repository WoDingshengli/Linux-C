/*将键盘回显开或关，如果命令行参数以‘y'开始
 *终端的回显标志开启，否则回显关闭
 */

 #include <stdio.h>
 #include <termios.h>
 #include <stdlib.h>

 #define oop(s, x) {perror(s), exit(x);}

 int main(int ac, char *av[])
 {
     struct termios info;

     if(tcgetattr(0, &info) == -1)  /*stdin的文件描述符为1*/
         oop("tcgetattr", 1);
     if(av[1][0] == 'y')
         info.c_lflag |= ECHO;
     else
         info.c_lflag &= ~ECHO; 

     if(tcsetattr(0, TCSANOW, &info) == -1)
        oop("tcsetattr", 2);

     char s;
     scanf("%c", &s);

     printf("Ok!%c\n", s);
     return 0;
}
