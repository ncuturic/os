#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<limits.h>
#include<dirent.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==4, "argumenti");
    int fd = open(argv[1], O_RDONLY);
    check_error(fd!=-1, "open");
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    check_error(lseek(fd, a, SEEK_SET)!=-1, "lseek");
    char* buff = (char*)malloc(b);
    check_error(buff!=NULL, "malloc");
    check_error(read(fd, buff, b)!=-1, "read");
    check_error(write(STDOUT_FILENO, buff, b)!=-1, "write");
    free(buff);
    exit(EXIT_SUCCESS);
}