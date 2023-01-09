#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define ARRAY_MAX 1024
typedef struct{
    sem_t inDataReady;
    float array[ARRAY_MAX];
    unsigned arrayLen;
}OsInputData;
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int fd = shm_open(argv[1], O_RDWR, 0);
    check_error(fd!=-1, "shm_open");
    struct stat fileinfo;
    check_error(fstat(fd, &fileinfo)!=-1, "stat");
    size_t size = fileinfo.st_size;
    OsInputData* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    check_error(data!=MAP_FAILED, "mmap");
    close(fd);
    float zbir = 0;
    check_error(sem_wait(&data->inDataReady)!=-1, "sem_wait");
    for(unsigned i=0;i<data->arrayLen;i++)zbir+=data->array[i];
    float prosek = zbir / data->arrayLen;
    printf("%.2f\n", prosek);
    check_error(munmap(data, size)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}