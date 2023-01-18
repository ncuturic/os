#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<ftw.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* dan(int x)
{
    if(x==0)return "nedelja";
    if(x==1)return "ponedeljak";
    if(x==2)return "utorak";
    if(x==3)return "sreda";
    if(x==4)return "cetvrtak";
    if(x==5)return "petak";
    if(x==6)return "subota";
}
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(typeflag==FTW_F)
    {
        time_t modTime = sb->st_mtim.tv_sec;
        struct tm* brokenTime = localtime(&modTime);
        printf("%s %s\n", fpath+ftwbuf->base, dan(brokenTime->tm_wday));
    }
    return 0;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "lstat");
    check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
    check_error(nftw(argv[1], fn, 50, 0)!=-1, "nftw");
    exit(EXIT_FAILURE);
}