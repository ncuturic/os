#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
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
double globalMin;
pthread_mutex_t globalLock;
int n, m;
void* thread_func(void* arg)
{
    double* niz = (double*)arg;
    double localMin = niz[0];
    for(int i=0;i<m;i++)localMin = niz[i] < localMin ? niz[i] : localMin;
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    globalMin = localMin < globalMin ? localMin : globalMin;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    fscanf(ulaz, "%d%d", &n, &m);
    double** mat = (double**)malloc(n*sizeof(double*));
    check_error(mat!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        mat[i] = (double*)malloc(m*sizeof(double));
        check_error(mat[i]!=NULL, "malloc");
        for(int j=0;j<m;j++)fscanf(ulaz, "%lf", &mat[i][j]);
    }
    globalMin = mat[0][0];
    pthread_t* tids = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(tids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<n;i++)thread_error(pthread_create(&tids[i], NULL, thread_func, mat[i]), "pthread_create");
    for(int i=0;i<n;i++)thread_error(pthread_join(tids[i], NULL), "pthread_join");
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    printf("%lf\n", globalMin);
    free(tids);
    for(int i=0;i<n;i++)free(mat[i]);
    free(mat);
    fclose(ulaz);
    exit(EXIT_SUCCESS);
}