/*
 *对文件write时开启自动添加模式
 */

 #include<fcntl.h>

 int s;  //file flag
 s = fcntl(fd, F_GETFL):
 s |= O_APPEND;
 result = fcntl(fd, F_SETFL, s);
 if(result == -1)
     perror("setting APPEND wrong!");
 else
     write(fd, &rec, 1);
