//Prolazi samo 30%
#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/time.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAX_BYTES 4096
int main(int argc, char* argv[])
{
    check_error(argc==3, "./a.out srcPath destPath");
    struct stat fileinfo;
    check_error(stat(argv[1], &fileinfo)!=-1, "stat");
    int fdSrc = open(argv[1], O_RDONLY);
    check_error(fdSrc!=-1, "open");
    int fdDest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    check_error(fdDest!=-1, "open");
    char* buff = (char*)malloc(MAX_BYTES);
    check_error(buff!=NULL, "malloc");
    ssize_t read_bytes = 0;
    while((read_bytes = read(fdSrc, buff, MAX_BYTES)) > 0)
    {
        check_error(write(fdDest, buff, MAX_BYTES)!=-1, "write");
    }
    check_error(read_bytes!=-1, "read");
    check_error(chmod(argv[2], fileinfo.st_mode)!=-1, "chmod");
    check_error(close(fdDest)!=-1, "close");
    check_error(close(fdSrc)!=-1, "close");
    check_error(unlink(argv[1])!=-1, "unlink");
    free(buff);
    exit(EXIT_SUCCESS);
}
