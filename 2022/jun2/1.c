#define _XOPEN_SOURCE 700
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<ftw.h>
#include<string.h>
#define greska(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s\n", (msg));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(typeflag==FTW_D)
    {
        char* c = strstr(fpath+(ftwbuf->base), "test_");
        if(c!=NULL && c-fpath==ftwbuf->base)
        {
            if(!(sb->st_mode & S_IWGRP) && !(sb->st_mode & S_IWOTH))
            {
                printf("%s\n", fpath+(ftwbuf->base));
            }
        }
    }
    return 0;
}
int main(int argc, char* argv[])
{
    greska(argc==2, "argumenti");
    struct stat fileinfo;
    greska(lstat(argv[1], &fileinfo)!=-1, "stat");
    greska(S_ISDIR(fileinfo.st_mode), "nije direktorijum");
    greska(nftw(argv[1], fn, 10, 0)!=-1, "nftw");
    exit(EXIT_SUCCESS);
}
