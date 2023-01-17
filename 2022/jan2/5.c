#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<errno.h>
#define MAXS 256
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int provera(char* s)
{
    int n = strlen(s);
    for(int i=0;i<n;i++)
    {
        if(!isupper(s[i]))return 0;
    }
    return 1;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* stream = fopen(argv[1], "r");
    check_error(stream!=NULL, "fopen");
    int fd = open(argv[1], O_RDWR);
    check_error(fd!=-1, "open");
    int cnt = 0;
    char rec[MAXS];
    while(fscanf(stream, "%s", rec)!=EOF)
    {
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_start = ftell(stream) - strlen(rec);
        lock.l_len = strlen(rec);
        lock.l_whence = SEEK_SET;
        int ret = fcntl(fd, F_SETLK, &lock);
        if(ret==-1)
        {
            check_error(errno==EAGAIN || errno==EACCES, "fcntl");
        }
        else 
        {
            if(provera(rec))
            {
                char buff[MAXS];
                memset(buff, '#', strlen(rec));
                check_error(lseek(fd, ftell(stream)-strlen(rec), SEEK_SET)!=-1, "lseek");
                check_error(write(fd, buff, strlen(rec))!=-1, "write");
                cnt += strlen(rec);
            }
        }
    }
    printf("%d\n", cnt);
    fclose(stream);
    close(fd);
    exit(EXIT_SUCCESS);
}