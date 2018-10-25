/*注销用户，修改utmp中相应到记录*/
/*参数为要注销到用户名*/

/*return -1 on error, 0 on success*/
int logout_tty(char *line)
{
    int fd = open(UTMP_FILE, O_RDWR);
    if(fd == -1) return -1;

    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    while(read(fd, &rec, len) == len)
        if(strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0)
        {
            rec.ut_type = DEAD_PROCESS; /*logout*/
            if(time(&rec.ut_time != -1))
                if( lseek(fd, -len,SEEK_CUR) != -1)  /*pointer of UTMP_FILE move*/
                    if(write(fd, &rec, len) == len)
                        retval = 0;
            break;
         }

     if(close(fd) == -1)
         retval = -1;

     return retval;
}


