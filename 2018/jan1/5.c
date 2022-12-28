#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<semaphore.h>
#include<ctype.h>
#define ARRAY_MAX 1024
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
typedef struct {
sem_t inDataReady;
sem_t dataProcessed;
char str[ARRAY_MAX];
}OsInputData;
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int fd = shm_open(argv[1], O_RDWR, 0);
    check_error(fd!=-1, "shm_open");
    struct stat fileinfo;
    check_error(fstat(fd, &fileinfo)!=-1, "stat");
    size_t size = fileinfo.st_size;
    OsInputData* data = (OsInputData*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    check_error(data!=MAP_FAILED, "mmap");
    close(fd);
    check_error(sem_wait(&(data->inDataReady))!=-1, "sem_wait");
    int n = strlen(data->str);
    for(int i=0;i<n;i++)
    {
        if(isupper(data->str[i]))tolower(data->str[i]);
        if(islower(data->str[i]))toupper(data->str[i]);
    }
    check_error(sem_post(&(data->dataProcessed))!=-1, "sem_post");
    check_error(munmap(data, size)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}