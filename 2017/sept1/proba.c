#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define ARRAY_MAX 1024
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
typedef struct
{
    sem_t inDataReady;
    int array[ARRAY_MAX];
    unsigned arrayLen;
}OsInputData;
int stepenTrojke(int x)
{
    while(x%3==0)x/=3;
    return (x==1);
}
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
    check_error(sem_wait(&data->inDataReady), "sem_wait");
    for(int i=0;i<data->arrayLen;i++)if(stepenTrojke(data->array[i]))printf("%d ", data->array[i]);
    printf("\n");
    check_error(munmap(data, size), "munmap");
    exit(EXIT_SUCCESS);
}