#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<pwd.h>
#include<grp.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s", (msg));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    struct stat fileinfo;
    check_error(stat(argv[1], &fileinfo)!=-1, "stat");
    struct passwd* user;
    check_error((user=getpwuid(fileinfo.st_uid))!=NULL, "getpwuid");
    printf("%s ", user->pw_name);
    struct group* group;
    check_error((group=getgrgid(fileinfo.st_gid))!=NULL, "getgrgid");
    printf("%s", group->gr_name);
    exit(EXIT_SUCCESS);
}