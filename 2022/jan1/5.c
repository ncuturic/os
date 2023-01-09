#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 256
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r+");
    check_error(ulaz!=NULL, "fopen");
    struct flock lock;
    char rec[MAXS];
    int rez = 0;
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    while(fscanf(ulaz, "%s", rec)!=EOF)
    {
        int len = strlen(rec);
        int trenutno = ftell(ulaz);
        lock.l_type = F_WRLCK;
        lock.l_start = trenutno - len;
        lock.l_len = len;
        lock.l_whence = SEEK_SET;
        check_error(fcntl(fd, F_GETLK, &lock)!=-1, "fcntl");
        if(lock.l_type == F_UNLCK)
        {
            char* kraj;
            strtol(rec, &kraj, 10);
            if(kraj!=NULL && *kraj==0)
            {
                fseek(ulaz, -len, SEEK_CUR);
                for(int i=0;i<len;i++)fprintf(ulaz, "#");
                rez += len;
            }
        }
    }
    fclose(ulaz);
    close(fd);
    printf("%d\n", rez);
    exit(EXIT_SUCCESS);
}