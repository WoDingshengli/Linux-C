/*实现字符串在屏幕上弹来弹去*/

#include <curses.h>
#include <unistd.h>

#define LEFTEDGE  10
#define RIGHTEDGE 30
#define ROW       10

int main()
{
    char *message = "Hello world!";
    char *blank   = "            ";
    int dir = 1;
    int pos = LEFTEDGE;

    initscr();
    clear();

    while(1){
        move(ROW , pos);
        addstr(message);
        move(LINES - 1, COLS - 1);
        refresh();
        usleep(10);
        move(ROW , pos);
        addstr(blank);
        pos += dir;
        if( pos >= RIGHTEDGE)
            dir = -1;
        if( pos <= LEFTEDGE)
            dir = +1;
    }

    return 0;
}
