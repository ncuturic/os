#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#define greska(cond, poruka) do{\
        if(!(cond)){\
            fprintf(stderr, "%s\n", (poruka));\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    greska(argc==2, "argumenti");
    struct stat fileinfo;
    greska(lstat(argv[1], &fileinfo)!=1, "stat");
    //st_uid st_gid
    struct passwd *korisnik = getpwuid(fileinfo.st_uid);
    greska(korisnik!=NULL, "getpwuid");
    printf("%s ", korisnik->pw_name);
    struct group *grupa = getgrgid(fileinfo.st_gid);
    greska(grupa!=NULL, "getgrgid");
    printf("%s\n", grupa->gr_name);
    exit(EXIT_SUCCESS);
}
