#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<time.h>
#include<string.h>
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
    char weekDay[NAME_MAX];
    int timeInSeconds;
}OsInputData;
char* dan(int x)
{
    if(x==0)return "nedelja";
    if(x==1)return "ponedeljak";
    if(x==2)return "utorak";
    if(x==3)return "sreda";
    if(x==4)return "cetvrtak";
    if(x==5)return "petak";
    if(x==6)return "subota";
}
OsInputData* openSharedMemory(char* memoryName, int* size)
{
    int fd = shm_open(memoryName, O_RDWR, 0);
    check_error(fd!=-1, "shm_open");
    struct stat fileinfo;
    check_error(fstat(fd, &fileinfo)!=-1, "fstat");
    *size = fileinfo.st_size;
    OsInputData* addr = (OsInputData*)mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
    struct tm* brokenTime = localtime(&data->timeInSeconds);
    check_error(brokenTime!=NULL, "localtime");
    strcpy(data->weekDay, dan(brokenTime->tm_wday));
    printf("%s\n", data->weekDay);
    check_error(sem_post(&data->operationDone)!=-1, "sem_post");
    check_error(munmap(data, size)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}