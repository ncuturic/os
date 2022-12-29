#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==4, "argumenti");
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=a;
    lock.l_len=b+1;
    check_error(fcntl(fd, F_GETLK, &lock)!=-1, "fcntl");
    if(lock.l_type==F_UNLCK)printf("unlocked\n");
    else if(lock.l_type==F_RDLCK)printf("shared lock\n");
    else if(lock.l_type==F_WRLCK)printf("exclusive lock\n");
    check_error(close(fd)!=-1, "close");
    exit(EXIT_SUCCESS);
}