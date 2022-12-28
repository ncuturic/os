#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<ftw.h>
#include<limits.h>
#include<time.h>
#define greska(cond, poruka) do{\
        if(!(cond)){\
            fprintf(stderr, "%s\n", (poruka));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* dan_nedelje(int x)
{
    if(x==0)return "nedelja";
    if(x==1)return "ponedeljak";
    if(x==2)return "utorak";
    if(x==3)return "sreda";
    if(x==4)return "cetvrtak";
    if(x==5)return "petak";
    if(x==6)return "subota";
    return NULL;
}
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(typeflag==FTW_F)
    {
        char* abspath = realpath(fpath, NULL);
        greska(abspath!=NULL, "realpath");
        struct tm *vreme = localtime(&(sb->st_mtim).tv_sec);
        greska(vreme!=NULL, "localtime");
        char* dan = dan_nedelje(vreme->tm_wday);
        greska(dan!=NULL, "dan_nedelje");
        printf("%s %s\n", abspath, dan);
        free(abspath);
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
