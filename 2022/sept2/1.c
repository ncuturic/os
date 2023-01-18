#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<ftw.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int rez;
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(typeflag==FTW_F && sb->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))rez += sb->st_size;
    return 0;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "lstat");
    check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
    check_error(nftw(argv[1], fn, 50, 0)!=-1, "nftw");
    printf("%d\n", rez);
    exit(EXIT_SUCCESS);
}