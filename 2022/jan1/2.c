#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<ftw.h>
#include<string.h>
#include<time.h>
#include<fcntl.h>
#define DANI (7*24*60*60)
char* ekstenzija;
void greska(char* poruka)
{
    fprintf(stderr, "%s\n", poruka);
    exit(EXIT_FAILURE);
}
int fn(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf)
{
    char* ext = strrchr(fpath, '.');
    time_t vreme = time(NULL);
    time_t izmena = (sb->st_mtim).tv_sec;
    if(!strcmp(ext, ekstenzija) && typeflag==FTW_F && vreme-izmena<DANI)printf("%s ", fpath+(ftwbuf->base));
    return 0;
}
int main(int argc, char* argv[])
{
    if(argc!=3)greska("losi argumenti");
    ekstenzija = argv[2];
    struct stat fileinfo;
    if(stat(argv[1], &fileinfo)==-1)greska("stat");
    if(!S_ISDIR(fileinfo.st_mode))greska("nije dat direktorijum");
    if(nftw(argv[1], fn, 50, 0)==-1)greska("nftw");
    exit(EXIT_SUCCESS);
}
