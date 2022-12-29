#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<errno.h>
#include<string.h>
#define MAX_BYTES 4096
#define check_error(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s", (msg));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    if(!strcmp(argv[1], "-r"))
    {
        int fd = open(argv[2], O_RDONLY);
        char* buff = (char*)malloc(MAX_BYTES);
        check_error(buff!=NULL, "malloc");
        ssize_t read_bytes = 0;
        while((read_bytes = read(fd, buff, MAX_BYTES)) > 0)
        {
            check_error(write(STDOUT_FILENO, buff, MAX_BYTES)!=-1, "write");
        }
        check_error(read_bytes!=-1, "read");
        close(fd);
        free(buff);
    }
    else if(!strcmp(argv[1], "-w") || !strcmp(argv[1], "-a"))
    {
        int flags = O_WRONLY | O_CREAT;
        if(!strcmp(argv[1], "-w"))flags |= O_TRUNC;
        if(!strcmp(argv[1], "-a"))flags |= O_APPEND;
        int fd = open(argv[2], flags);
        char* buff = (char*)malloc(MAX_BYTES);
        check_error(buff!=NULL, "malloc");
        ssize_t read_bytes = 0;
        while((read_bytes = read(STDIN_FILENO, buff, MAX_BYTES)) > 0)
        {
            check_error(write(fd, buff, MAX_BYTES)!=-1, "write");
        }
        check_error(read_bytes!=-1, "read");
        close(fd);
        free(buff);
    }
    else
    {
        check_error(0, "los prvi argument");
    }
    exit(EXIT_SUCCESS);
}