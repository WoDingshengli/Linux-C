/*
version 0.1 of more
如果在命令行中给出了文件名，那么就分页显示这个文件，否则从标准输入得到想要分页显示到内容;
*/

#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);

int see_more(FILE *);

int main(int ac, char **av)
{
    FILE * fp = NULL;
    if( ac == 1) do_more(stdin);
    else
        while(--ac)
        if((fp = fopen(*++av, "r")) != NULL)
        {
            do_more( fp );
            fclose( fp );
        }
        else
            exit(1);
    return 0;
}

void do_more(FILE * fp)
/*
 * read PAGELEN lines, then call see_more() for further instructions
 */
{
    char line[LINELEN];
    int num_of_lines = 0;
    int reply;
    FILE * fp_tty;
    fp_tty = fopen("/dev/tty", "r");
    if( fp_tty == NULL) exit(1);
    while(fgets( line, LINELEN, fp )){
        if(num_of_lines == PAGELEN){
            reply = see_more(fp_tty);
            if(reply == 0) break;
            num_of_lines -= reply;
         }
         if( fputs(line, stdout) == EOF ) exit(1);
         num_of_lines++;
        }
}

int see_more(FILE * cmd)
{
    int c;
    printf("more?");
    while((c = getc(cmd)) != EOF)
    {
        switch(c){
            case 'q': return 0;
            case ' ': return PAGELEN;
            case '\n': return 1;
            default: printf("Wrong! Please entry again:");
        }
    }
}
        
