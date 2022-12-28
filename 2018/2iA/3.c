#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "stat");
    unsigned newMode = (fileinfo.st_mode & S_IRWXU);
    if(fileinfo.st_mode & S_IRGRP)newMode |= S_IROTH;
    if(fileinfo.st_mode & S_IWGRP)newMode |= S_IWOTH;
    if(fileinfo.st_mode & S_IXGRP)newMode |= S_IXOTH;

    if(fileinfo.st_mode & S_IROTH)newMode |= S_IRGRP;
    if(fileinfo.st_mode & S_IWOTH)newMode |= S_IWGRP;
    if(fileinfo.st_mode & S_IXOTH)newMode |= S_IXGRP;
    check_error(chmod(argv[1], newMode)!=-1, "chmod");
    exit(EXIT_SUCCESS);
}