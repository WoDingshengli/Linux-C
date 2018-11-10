/*模仿ATM的交易过程
 *使用非阻塞模式实现超时响应
 *忽略退出键
 *对中断信号进行处理
 */

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

#define ASK "Do you want another transaction(y/n)?"
#define TRIES 3   /*用户所能尝试的最多次数*/
#define SLEEPTIME 2
#define BEEP putchar('\a')  /*提醒用户*/

void tty_mode(int how);
char get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();
int  get_response(char *, int );
void ctrl_c_handler(int );

int main(int ac, char **ag)
{
    int response;
    tty_mode(0);   
    set_cr_noecho_mode();
    set_nodelay_mode();
    signal(SIGINT, ctrl_c_handler);/*中断处理*/
    signal(SIGQUIT, SIG_IGN);/*忽略退出信号*/
    response = get_response(ASK, TRIES);
    tty_mode(1);
    return response;
}

void tty_mode(int how)
{
    static struct termios original_mode;
    static int original_flags;
    if(how == 0)
    {
        tcgetattr(0 , &original_mode);
        original_flags = fcntl(0 , F_GETFL);
    }
    else{
        tcsetattr(0 , TCSANOW, &original_mode);
        fcntl( 0, F_SETFL, original_flags);
   }

}  /*不等数据传输完毕立即改变属性为原先的模式*/

void set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr( 0, &ttystate);
    ttystate.c_lflag  &= ~ICANON; /*没有缓冲*/
    ttystate.c_lflag  &= ~ECHO; /*没有回显*/
    ttystate.c_cc[VMIN]  = 1; /*一次读取一个字符*/
    tcsetattr( 0, TCSANOW, &ttystate);
}

void set_nodelay_mode()
{
    int termflags;
    termflags = fcntl( 0, F_GETFL);
    termflags |= O_NDELAY;
    fcntl( 0, F_SETFL, termflags);
} /*非阻塞*/

void ctrl_c_handler(int signum)
{
    tty_mode(1);
    exit(1);
}

int get_response(char *question, int maxtries)
{
    int input;
    printf("%s", question);
    fflush(stdout);
    while(1)
    {
        sleep(SLEEPTIME); /*等一会*/
        input = tolower(get_ok_char()); 
        if(input == 'y') return 0;
        if(input == 'n') return 1;
        if(maxtries-- == 0) return 2;
        BEEP; /*提醒*/
     }
}

char get_ok_char()
{
    int c;
    while( (c = getchar()) != EOF && strchr("yYnN", c) == NULL);
    return c;
}

