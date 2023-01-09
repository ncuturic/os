#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#define ARRAY_MAX 10000
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
typedef struct {
    sem_t inDataReady;
    int array[ARRAY_MAX];
    unsigned arrayLen;
}OsData;
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    int ulazFd = shm_open(argv[1], O_RDWR | O_CREAT, 0600);
    check_error(ulazFd!=-1, "shm_open");
    check_error(ftruncate(ulazFd, sizeof(OsData))!=-1, "truncate");
    OsData* ulazni = (OsData*)mmap(NULL, sizeof(OsData), PROT_READ | PROT_WRITE, MAP_SHARED, ulazFd, 0);
    check_error(ulazni!=MAP_FAILED, "mmap");
    close(ulazFd);
    int izlazFd = shm_open(argv[2], O_RDWR | O_CREAT, 0600);
    check_error(izlazFd!=-1, "shm_open");
    check_error(ftruncate(izlazFd, sizeof(OsData))!=-1, "truncate");
    OsData* izlazni = (OsData*)mmap(NULL, sizeof(OsData), PROT_READ | PROT_WRITE, MAP_SHARED, izlazFd, 0);
    check_error(izlazni!=MAP_FAILED, "mmap");
    close(izlazFd);
    check_error(sem_init(&ulazni->inDataReady, 1, 0)!=-1, "sem_init");
    check_error(sem_init(&izlazni->inDataReady, 1, 0)!=-1, "sem_init");
    int n;
    scanf("%d", &n);
    ulazni->arrayLen=n;
    for(int i=0;i<n;i++)scanf("%d", &ulazni->array[i]);
    scanf("%d", &n);
    izlazni->arrayLen = n;
    for(int i=0;i<n;i++)scanf("%d", &izlazni->array[i]);
    check_error(sem_post(&ulazni->inDataReady)!=-1, "sem_post");
    check_error(sem_wait(&izlazni->inDataReady)!=-1, "sem_wait");
    for(unsigned i=0;i<izlazni->arrayLen;i++)printf("%d ", izlazni->array[i]);
    printf("\n");
    check_error(munmap(ulazni, sizeof(OsData))!=-1, "munmap");
    check_error(munmap(izlazni, sizeof(OsData))!=-1, "munmap");
    sleep(30);
    check_error(shm_unlink(argv[1])!=-1, "unlink");
    check_error(shm_unlink(argv[2])!=-1, "unlink");
    exit(EXIT_SUCCESS);
}