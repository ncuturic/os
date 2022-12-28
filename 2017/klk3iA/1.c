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
char* f(const char* file)
{
    struct stat fileinfo;
    check_error(lstat(file, &fileinfo)!=-1, "stat");
    check_error(S_ISREG(fileinfo.st_mode) || S_ISDIR(fileinfo.st_mode), "nije dobar fajl");
    char *res = (char*)malloc(11);
    check_error(res!=NULL, "malloc");
    if(S_ISDIR(fileinfo.st_mode))res[0]='d';
    else res[0]='-';
    if(fileinfo.st_mode&S_IRUSR)res[1]='r';
    else res[1]='-';
    if(fileinfo.st_mode&S_IWUSR)res[2]='w';
    else res[2]='-';
    if(fileinfo.st_mode&S_IXUSR)res[3]='x';
    else res[3]='-';
    if(fileinfo.st_mode&S_IRGRP)res[4]='r';
    else res[4]='-';
    if(fileinfo.st_mode&S_IWGRP)res[5]='w';
    else res[5]='-';
    if(fileinfo.st_mode&S_IXGRP)res[6]='x';
    else res[6]='-';
    if(fileinfo.st_mode&S_IROTH)res[7]='r';
    else res[7]='-';
    if(fileinfo.st_mode&S_IWOTH)res[8]='w';
    else res[8]='-';
    if(fileinfo.st_mode&S_IXOTH)res[9]='x';
    else res[9]='-';
    res[10]=0;
    return res;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    char* res;
    printf("%s", res=f(argv[1]));
    free(res);
    exit(EXIT_SUCCESS);
}