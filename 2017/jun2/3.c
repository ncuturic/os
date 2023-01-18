#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
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
    check_error(argc==4, "argumenti");
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    FILE* stream = fopen(argv[1], "r+");
    check_error(stream!=NULL, "fopen");
    char rec[MAXS];
    int cnt = 0;
    while(fscanf(stream, "%s", rec)!=EOF)
    {
        if(!strcmp(rec, argv[2]))
        {
            check_error(lseek(fd, ftell(stream)-strlen(rec), SEEK_SET)!=-1, "lseek");
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_len = strlen(rec);
            lock.l_whence = SEEK_CUR;
            int res = fcntl(fd, F_SETLK, &lock);
            if(res==-1)
            {
                check_error((errno==EAGAIN || errno==EACCES), "fcntl");
                cnt++;
            }
            else
            {
                check_error(write(fd, argv[3], strlen(argv[3]))!=-1, "write");
            }
        }
    }
    fclose(stream);
    close(fd);
    printf("%d\n", cnt);
    exit(EXIT_SUCCESS);
}