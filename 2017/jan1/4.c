#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv)
{
    check_error(argc==4, "argumenti");
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    int fd = open(argv[1], O_RDONLY);
    check_error(fd!=-1, "open");
    struct flock lck;
    lck.l_type = F_GETLK;
    lck.l_whence = SEEK_SET;
    lck.l_start = a;
    lck.l_len = b;
    check_error(fcntl(fd, )-1, "fcntl");
    exit(EXIT_SUCCESS);
}