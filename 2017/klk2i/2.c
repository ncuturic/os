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
void f(const char* dname)
{
    mode_t oldMask = umask(0);
    check_error(mkdir(dname, 0777)!=-1, "mkdir");
    umask(oldMask);
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    f(argv[1]);
    exit(EXIT_SUCCESS);
}