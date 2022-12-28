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
#include<ftw.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* ext;
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    char* fileExt = strrchr(fpath, '.');
    if(typeflag==FTW_F && ftwbuf->level>=2 && ftwbuf->level<=5 && fileExt!=NULL && strcmp(ext, fileExt)==0)
    {
        printf("%s\n", fpath+ftwbuf->base);
    }
    return 0;
}
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    ext = argv[2];
    struct stat fileinfo;
    check_error(lstat(argv[1], &fileinfo)!=-1, "stat");
    check_error(S_ISDIR(fileinfo.st_mode), "not a dir");
    check_error(nftw(argv[1], fn, 50, 0)!=-1, "nftw");
    exit(EXIT_SUCCESS);
}