#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<limits.h>
#define MAXS 4096
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define thread_error(func, msg) do{\
        int privremeno = (func);\
        if(privremeno){\
            errno = privremeno;\
            check_error(0, msg);\
        }\
    }while(0)
int max = INT_MIN;
pthread_mutex_t globalLock;
typedef struct dt
{
    int n;
    int* niz;
}Data;
void* thread_func(void* arg)
{
    Data* data = (Data*)arg;
    int n = data->n;
    int* niz = data->niz;
    int localMax = niz[0];
    for(int i=1;i<n;i++)localMax = niz[i] > localMax ? niz[i] : localMax;
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    if(localMax > max)max = localMax;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    int n;
    fscanf(ulaz, "%d", &n);
    int** nizovi = (int**)malloc(n*sizeof(int*));
    check_error(nizovi!=NULL, "malloc");
    Data* thread_data = (Data*)malloc(n*sizeof(Data));
    check_error(thread_data!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        int m;
        fscanf(ulaz, "%d", &m);
        nizovi[i] = (int*)malloc(m*sizeof(int));
        check_error(nizovi[i]!=NULL, "malloc");
        for(int j = 0;j<m;j++)fscanf(ulaz, "%d", &nizovi[i][j]);
        thread_data[i].n = m;
        thread_data[i].niz = nizovi[i];
    }
    pthread_t* pids = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(pids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<n;i++)thread_error(pthread_create(&pids[i], NULL, thread_func, &thread_data[i]), "pthread_create");
    for(int i=0;i<n;i++)thread_error(pthread_join(pids[i], NULL), "pthread_join");
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    free(pids);
    for(int i=0;i<n;i++)free(nizovi[i]);
    free(nizovi);
    free(thread_data);
    fclose(ulaz);
    printf("%d\n", max);
    exit(EXIT_SUCCESS);
}