/*linux command -- who
 *who的第二个版本
 *1.消除空白记录
   utmp数据结构到成员ut_type为7（USER_PROCESS)时，表时这已经是一个登录到用户
 *2.正确显示登录时间
    使用命令man -k time
    使用ctime()
 */

#include<stdio.h>
#include<stdlib.h>
#include<utmp.h>
#include<time.h>
#include<fcntl.h>  /*include the api open*/
#include<unistd.h> /*include the api read , close*/

void show_info(struct utmp * );
void show_time(time_t time);

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
    if(u->ut_type != USER_PROCESS) return;
    printf("%-8s", u->ut_name);
    printf(" ");
    printf("%-8s", u->ut_line);
    printf(" ");
    show_time(u->ut_time);  /*the type of ut_time is time_t*/
    printf(" ");
  #ifndef SHOWHOST    /*show host*/
  #define SHOWHOST
    if(u->ut_host[0] != '\0')
    printf("( %s )", u->ut_host);
  #endif
    printf("\n");
}
    
void show_time(time_t time)
{
    char *t = ctime(&time);
    printf("%12s", t+4);
}
