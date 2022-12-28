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
    check_error(argc>1, "argumenti");
    struct stat fileinfo;
    for(int i=1;i<argc;i++)
    {
        char* absPath = realpath(argv[i], NULL);
        check_error(absPath!=NULL, "realpath");
        check_error(lstat(absPath, &fileinfo)!=-1, "stat");
        check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
        if(!(fileinfo.st_mode & S_IRWXO))printf("%ld\n", strlen(absPath));
        free(absPath);
    }
    exit(EXIT_SUCCESS);
}