//Radi samo 20%
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define ARRAY_MAX 10000
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
}OsData;
int brJedinica(int x)
{
    int cnt = 0;
    while(x)
    {
        if(x%2==1)cnt++;
        x/=2;
    }
    return cnt;
}
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    int ulazFd = shm_open(argv[1], O_RDWR, 0600);
    check_error(ulazFd!=-1, "shm_open");
    int izlazFd = shm_open(argv[2], O_RDWR | O_CREAT, 0600);
    check_error(izlazFd!=-1, "shm_open");
    struct stat fileinfo;
    check_error(fstat(ulazFd, &fileinfo)!=-1, "stat");
    size_t ulazniSize = fileinfo.st_size;
    OsData* ulazni = (OsData*)mmap(NULL, ulazniSize, PROT_READ | PROT_WRITE, MAP_SHARED, ulazFd, 0);
    check_error(ulazni!=MAP_FAILED, "mmap");
    close(ulazFd);
    check_error(fstat(izlazFd, &fileinfo)!=-1, "stat");
    size_t izlazniSize = fileinfo.st_size;
    OsData* izlazni = (OsData*)mmap(NULL, izlazniSize, PROT_READ | PROT_WRITE, MAP_SHARED, izlazFd, 0);
    check_error(izlazni!=MAP_FAILED, "mmap");
    close(izlazFd);
    int cnt = 0;
    check_error(sem_wait(&ulazni->inDataReady)!=-1, "sem_wait");
    for(int i=0;i<ulazni->arrayLen;i++)
    {
        if(brJedinica(ulazni->array[i])>=4)
        {
            izlazni->array[cnt++]=ulazni->array[i];
        }
    }
    izlazni->arrayLen=cnt;
    check_error(sem_post(&izlazni->inDataReady)!=-1, "sem_post");
    check_error(munmap(ulazni, ulazniSize)!=-1, "munmap");
    check_error(munmap(izlazni, izlazniSize)!=-1, "munmap");
    exit(EXIT_SUCCESS);
}
