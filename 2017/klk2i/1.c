#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s", (msg));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
bool f(const char* fpath)
{
    struct stat fileinfo;
    check_error(lstat(fpath, &fileinfo)!=-1, "stat");
    check_error(S_ISREG(fileinfo.st_mode), "nije regularan fajl");
    unsigned mask = S_IROTH | S_IWOTH;
    return ((fileinfo.st_mode & mask) == mask);
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    bool res = f(argv[1]);
    if(res)printf("true");
    else printf("false");
    exit(EXIT_SUCCESS);
}