#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
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
    char rec[MAXS];
    char* const test = "test_";
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    while(fscanf(ulaz, "%s", rec)!=EOF)
    {
        if(strstr(rec, test)==rec)
        {
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_start = ftell(ulaz);
            lock.l_len = -strlen(rec);
            lock.l_whence = SEEK_SET;
            int tmp = fcntl(fd, F_SETLK, &lock);
            if(tmp==-1 && errno!=EACCES && errno!=EAGAIN)check_error(0, "fcntl");
            else if(tmp!=-1)
            {
                check_error(fseek(ulaz, ftell(ulaz)-strlen(rec), SEEK_SET)!=-1, "fseek");
                char* output = (char*)malloc(strlen(rec)+1);
                check_error(output!=NULL, "malloc");
                memset(output, '#', strlen(rec));
                output[strlen(rec)] = 0;
                fprintf(ulaz, "%s ", output);
                free(output);
            }
        }
    }
    fclose(ulaz);
    close(fd);
    exit(EXIT_SUCCESS);
}