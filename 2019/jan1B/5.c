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
    check_error(argc==2, "argumenti");
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    FILE* file = fopen(argv[1], "r");
    check_error(file!=NULL, "fopen");
    char rec[MAXS];
    int idx = 0;
    while(fscanf(file, "%s", rec)!=EOF)
    {
        char* tmp;
        if(strlen(rec)==4)
        {
            int br = strtol(rec, &tmp, 10);
            if(*tmp==0 && br>0)
            {
                struct flock lock;
                lock.l_type=F_WRLCK;
                lock.l_whence=SEEK_SET;
                lock.l_start=idx;
                lock.l_len=strlen(rec);
                if(fcntl(fd, F_SETLK, &lock)!=-1)
                {
                    check_error(lseek(fd, idx, SEEK_SET)!=-1, "lseek");
                    check_error(write(fd, "####", 4)!=-1, "write");
                }
            }
        }
        idx += strlen(rec) + 1;
    }
    fclose(file);
    check_error(close(fd)!=-1, "close");
    exit(EXIT_SUCCESS);
}