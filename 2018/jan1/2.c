#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int duzinaCitanja, totalCnt;
char* w;
pthread_mutex_t globalLock;
void* thread_func(void* arg)
{
    char* buff = (char*)malloc(duzinaCitanja+1);
    check_error(buff!=NULL, "malloc");
    check_error(read(*((int*)arg), buff, duzinaCitanja)!=-1, "read");
    buff[duzinaCitanja] = 0;
    int cnt = 0;
    while((buff = strstr(buff, w))!=NULL)
    {
        buff++;
        cnt++;
    }
    free(buff);
    pthread_mutex_lock(&globalLock);
    totalCnt+=cnt;
    pthread_mutex_unlock(&globalLock);
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==4, "argumenti");
    struct stat fileinfo;
    check_error(stat(argv[1], &fileinfo)!=-1, "stat");
    check_error(S_ISREG(fileinfo.st_mode), "nije regularni fajl");
    size_t n = fileinfo.st_size;
    w = argv[2];
    int k = atoi(argv[3]);
    duzinaCitanja = n/k;
    int* fds = (int*)malloc(k*sizeof(int));
    check_error(fds!=NULL, "malloc");
    pthread_t* tid = (pthread_t*)malloc(k*sizeof(pthread_t));
    pthread_mutex_init(&globalLock, NULL);
    for(int i=0;i<k;i++)
    {
        fds[i] = open(argv[1], O_RDONLY);
        check_error(fds[i]!=-1, "open");
        check_error(lseek(fds[i], i*n/k, SEEK_SET)!=-1, "lseek");
        pthread_create(&tid[i], NULL, thread_func, &fds[i]);
    }
    for(int i=0;i<k;i++)
    {
        pthread_join(tid[i], NULL);
        close(fds[i]);
    }
    free(fds);
    free(tid);
    pthread_mutex_destroy(&globalLock);
    printf("%d\n", totalCnt);
    exit(EXIT_SUCCESS);
}