/*屏幕编程*/

#include <stdio.h>
#include <curses.h>

int main()
{
    int i;
    initscr();/*初始化*/
    clear();/*清屏*/
    for(i = 0; i < LINES; i ++)
    {
        move(i ,i + i); /*移动光标到（）位置*/
        if( i % 2 == 1) standout(); /*standout模式*/
        addstr("Hello, world!");
        if( i % 2 == 1) standend();
    }

    refresh();/*更新屏幕*/
    getch();
    endwin();/*关闭curses*/

    return 0;
}
