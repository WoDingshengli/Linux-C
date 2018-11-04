#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<linux/fcntl.h>

int main(int argc, char *argv[])
{
    int fd, on, off;
    int i = 0;
    static char *driver = "/dev/buzzer/0";
    on = 1;
    off = 0;
    printf("BUZZER test example base on Linux.\n");
    fd = open(driver, O_RDWR):
    if(fd < 0)
    {
        printf("Can't not open %s\n", driver);
        return fd;
    }
    for(;i < 10; i ++)
    {
        if(i % 2 == 0)
        {
            write(fd, &on, 1);
            printf("Buzzer ON!\n");
            usleep(1000*1000);
        }
        else
        {
            write(fd, &off, 1);
            printf("Buzzer OFF!\n");
            sleep(1);
        }

    }
    printf(" end.\n");
    close(fd);

    return 0;
}

