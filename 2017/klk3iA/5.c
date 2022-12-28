#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/time.h>
#include<time.h>
#include<ftw.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int brojaci[8];
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(S_ISREG(sb->st_mode))brojaci[0]++;
    if(S_ISDIR(sb->st_mode))brojaci[1]++;
    if(S_ISCHR(sb->st_mode))brojaci[2]++;
    if(S_ISBLK(sb->st_mode))brojaci[3]++;
    if(S_ISLNK(sb->st_mode))brojaci[4]++;
    if(S_ISFIFO(sb->st_mode))brojaci[5]++;
    if(S_ISSOCK(sb->st_mode))brojaci[6]++;
    brojaci[7]++;
    return 0;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "./a.out dirPath");
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "stat");
    check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
    check_error(nftw(argv[1], fn, 10, FTW_PHYS)!=-1, "nftw");
    for(int i=0;i<8;i++)printf("%d ", brojaci[i]);
    exit(EXIT_SUCCESS);
}
