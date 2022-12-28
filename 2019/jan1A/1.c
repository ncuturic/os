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
char* danNedelje(int dan)
{
    if(dan==0)return "nedelja";
    if(dan==1)return "ponedeljak";
    if(dan==2)return "utorak";
    if(dan==3)return "sreda";
    if(dan==4)return "cetvrtak";
    if(dan==5)return "petak";
    return "subota";
}
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    time_t sekunde = atoll(argv[1]);
    int godine = atoi(argv[2]);
    struct tm* vreme = localtime(&sekunde);
    printf("%s ", danNedelje(vreme->tm_wday));
    vreme->tm_year += godine;
    check_error(mktime(vreme)!=-1, "mktime");
    printf("%s ", danNedelje(vreme->tm_wday));
    exit(EXIT_SUCCESS);
}
