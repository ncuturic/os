#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define NAME_MAX 256
typedef struct
{
    sem_t inDataReady;
    sem_t operationDone;
    char linkName[NAME_MAX];
    int linkSize;
}OsInputData;
OsInputData* openSharedMemory(char* memoryName, int* size)
{
    int fd = shm_open(memoryName, O_RDWR, 0);
    check_error(fd!=-1, "shm_open");
    struct stat fileinfo;
    check_error(fstat(fd, &fileinfo)!=-1, "fstat");
    *size = fileinfo.st_size;
    OsInputData* addr = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    check_error(addr!=MAP_FAILED, "mmap");
    close(fd);
    return addr;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int size = 0;
    OsInputData* data = openSharedMemory(argv[1], &size);
    check_error(sem_wait(&data->inDataReady)!=-1, "sem_wait");
    struct stat fileinfo;
    int ret = lstat(data->linkName, &fileinfo);
    if(ret==-1)data->linkSize=-1;
    else data->linkSize = fileinfo.st_size;
    printf("%d\n", data->linkSize);
    check_error(sem_post(&data->operationDone)!=-1, "sem_post");
    check_error(munmap(data, size)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}
