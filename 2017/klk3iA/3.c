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
#define check_error(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s", (msg));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==4, "argumenti");
    int prava = strtol(argv[3], NULL, 8);
    mode_t oldMask = umask(0);
    int uspeo = 0;
    if(strcmp(argv[1], "-f")==0)
    {
        uspeo = 1;
        errno = 0;
        int fd;
        if((fd = open(argv[2], O_RDONLY | O_CREAT | O_EXCL, prava)) == -1)
        {
            umask(oldMask);
            check_error(errno==EEXIST, "open");
            check_error(chmod(argv[2], prava)!=-1, "chmod");
        }
        else umask(oldMask);
        close(fd);
    }
    else if(strcmp(argv[1], "-d")==0)
    {
        uspeo = 1;
        errno = 0;
        if(mkdir(argv[2], prava) == -1)
        {
            umask(oldMask);
            check_error(errno==EEXIST, "mkdir");
            check_error(chmod(argv[2], prava)!=-1, "chmod");
        }
        else umask(oldMask);
    }
    if(!uspeo)
    {
        umask(oldMask);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}