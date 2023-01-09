#define _XOPEN_SOURCE 700
#define MAXS 1024
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    int fdDest = open(argv[2], O_WRONLY);
    check_error(fdDest!=-1, "open");
    off_t off;
    char s[MAXS];
    while(fscanf(ulaz, "%ld%s", &off, s)!=EOF)
    {
        check_error(lseek(fdDest, off, SEEK_SET)!=-1, "lseek");
        check_error(write(fdDest, s, strlen(s))!=-1, "write");
    }
    close(fdDest);
    fclose(ulaz);
    exit(EXIT_SUCCESS);
}