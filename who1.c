/*linux command -- who
 *who的难点：
 *1.从文件utmp中读取相关数据结构
 *2.将结构到信息以合适到形式显示出来
 *可以用 man 5 utmp 在ubantu中查看utmp数据结构
 */

#include<stdio.h>
#include<stdlib.h>
#include<utmp.h>
#include<fcntl.h>  /*include the api open*/
#include<unistd.h> /*include the api read , close*/

void show_info(struct utmp * );

int main()
{
    struct utmp current_record; /* read information into here */
    int    utmpfd; /*file descriptor, int type*/
    int    reclen = sizeof(current_record); /*received length*/

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){    /*read only from utmp file, utmp file stores
                                                        data according to its datastructure*/
        perror( UTMP_FILE );
        exit(1);
    }

    while( read(utmpfd, &current_record, reclen) == reclen )
        show_info(&current_record); /*displays contents of the utmp file in human readable form*/
    close(utmpfd);

    return 0;
}

void show_info(struct utmp * u)
{
    printf("%-8s", u->ut_name);
    printf(" ");
    printf("%-8s", u->ut_line);
    printf(" ");
    printf(" % 10d", u->ut_time);
    printf(" ");
  #ifndef SHOWHOST    /*show host*/
  #define SHOWHOST
    printf("( %s )", u->ut_host);
  #endif
    printf("\n");
}
    




