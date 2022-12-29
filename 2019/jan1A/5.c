#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#define MAXS 256
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    char rec[MAXS];
    int idx = 0;
    int length = strlen(argv[2]);
    while(fscanf(ulaz, "%s", rec)!=EOF)
    {
        if(strcmp(rec, argv[2])==0)
        {
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = idx;
            lock.l_len = length;
            check_error(fcntl(fd, F_GETLK, &lock)!=-1, "fcntl");
            if(lock.l_type==F_RDLCK)printf("%d r\n", idx);
            else if(lock.l_type==F_WRLCK)printf("%d w\n", idx);
        }
        idx+=strlen(rec)+1;
    }
    fclose(ulaz);
    check_error(close(fd)!=-1, "close");
    exit(EXIT_SUCCESS);
}