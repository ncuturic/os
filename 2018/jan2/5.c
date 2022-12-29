#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<string.h>
#include<unistd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define ARRAY_MAX 1024
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
    check_error(sem_wait(&data->inDataReady)!=-1, "sem_wait");
    int n = strlen(data->str);
    char* tmp = (char*)malloc(n);
    check_error(tmp!=NULL, "malloc");
    for(int i=n-1;i>=0;i--)tmp[n-1-i] = data->str[i];
    tmp[n] = 0;
    strcpy(data->str, tmp);
    check_error(sem_post(&data->dataProcessed)!=-1, "sem_post");
    free(tmp);
    check_error(munmap(data, size)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}