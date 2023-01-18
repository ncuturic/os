#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int fd = open(argv[1], O_RDONLY);
    check_error(fd!=-1, "open");
    struct stat fileinfo;
    check_error(stat(argv[1], &fileinfo)!=-1, "stat");
    struct flock lock;
    int cnt = 0;
    for(int i=0;i<fileinfo.st_size;i++)
    {
        lock.l_type = F_WRLCK;
        lock.l_start = i;
        lock.l_len = 1;
        lock.l_whence = SEEK_SET;
        check_error(fcntl(fd, F_GETLK, &lock)!=-1, "fcntl");
        if(lock.l_type!=F_UNLCK)cnt++;
    }
    printf("%d\n", cnt);
    close(fd);
    exit(EXIT_SUCCESS);
}