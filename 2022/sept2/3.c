#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<limits.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define thread_error(func, msg) do{\
        int __tmp = (func);\
        if(__tmp){\
            errno = __tmp;\
            check_error(0, msg);\
        }\
    }while(0)
int n, m, globalMax = INT_MIN;
pthread_mutex_t globalLock;
void* thread_func(void* arg)
{
    int* sum = (int*)malloc(sizeof(int));
    check_error(sum!=NULL, "malloc");
    int* kolona = (int*)arg;
    for(int i=0;i<n;i++)*sum += kolona[i];
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    globalMax = *sum > globalMax ? *sum : globalMax;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return sum;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    fscanf(ulaz, "%d%d", &n, &m);
    int** mat = (int**)malloc(m*sizeof(int*));
    check_error(mat!=NULL, "malloc");
    for(int i=0;i<m;i++)
    {
        mat[i] = (int*)malloc(n*sizeof(int));
        check_error(mat[i]!=NULL, "malloc");
    }
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)fscanf(ulaz, "%d", &mat[j][i]);
    pthread_t* tids = (pthread_t*)malloc(m*sizeof(pthread_t));
    check_error(tids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<m;i++)thread_error(pthread_create(&tids[i], NULL, thread_func, mat[i]), "pthread_create");
    for(int i=0;i<m;i++)
    {
        void* res;
        thread_error(pthread_join(tids[i], &res), "pthread_join");
        printf("%d ", *(int*)res);
        free(res);
    }
    printf("%d\n", globalMax);
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    free(tids);
    for(int i=0;i<m;i++)free(mat[i]);
    free(mat);
    fclose(ulaz);
    exit(EXIT_SUCCESS);
}