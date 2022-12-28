#define _XOPEN_SOURCE 700
#define MAXS 1024
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
void obilazak(char* fpath, int* minSize, int* maxSize)
{
    DIR* dir = opendir(fpath);
    check_error(dir!=NULL, "opendir");
    struct dirent* dirEntry = NULL;
    while((dirEntry=readdir(dir))!=NULL)
    {
        char* putanja = (char*)malloc(strlen(fpath) + strlen(dirEntry->d_name) + 2);
        check_error(putanja!=NULL, "malloc");
        strcpy(putanja, fpath);
        strcat(putanja, "/");
        strcat(putanja, dirEntry->d_name);
        struct stat fileinfo;
        check_error(lstat(putanja, &fileinfo)!=-1, "stat");
        if(S_ISREG(fileinfo.st_mode))
        {
            if(fileinfo.st_size < *minSize)*minSize = fileinfo.st_size;
            if(fileinfo.st_size > *maxSize)*maxSize = fileinfo.st_size;
        }
        if(S_ISDIR(fileinfo.st_mode) && strcmp(dirEntry->d_name, ".")!=0 && strcmp(dirEntry->d_name, "..")!=0)
        {
            obilazak(putanja, minSize, maxSize);
        }
    }
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "stat");
    check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
    int minSize = INT_MAX, maxSize = 0;
    obilazak(argv[1], &minSize, &maxSize);
    printf("%d", maxSize-minSize);
    exit(EXIT_SUCCESS);
}