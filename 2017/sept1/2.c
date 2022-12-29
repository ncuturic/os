#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAXS 64
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int res, maxDeg;
pthread_mutex_t globalLock;
void* thread_func(void* arg)
{
    int cnt = 0;
    for(int i=2;i<((int*)arg)[0]+2;i++)cnt+=((int*)arg)[i];
    pthread_mutex_lock(&globalLock);
    if(cnt>maxDeg)
    {
        maxDeg = cnt;
        res = ((int*)arg)[1];
    }
    pthread_mutex_unlock(&globalLock);
    return NULL;
}
int main()
{
    int n;
    scanf("%d", &n);
    int** mat = (int**)malloc(n*sizeof(int*));
    check_error(mat!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        mat[i] = (int*)malloc(n*sizeof(int));
        check_error(mat[i]!=NULL, "malloc");
    }
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)scanf("%d", &mat[i][j]);
    int** nizovi = (int**)malloc(n*sizeof(int*));
    check_error(nizovi!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        nizovi[i] = (int*)malloc((n+2)*sizeof(int));
        check_error(nizovi[i]!=NULL, "malloc");
    }
    for(int i=0;i<n;i++)
    {
        nizovi[i][0] = n;
        nizovi[i][1] = i;
        for(int j=0;j<n;j++)
        {
            nizovi[i][j+2] = mat[j][i];
        }
    }
    pthread_mutex_init(&globalLock, NULL);
    pthread_t *tid = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(tid!=NULL, "malloc");
    for(int i=0;i<n;i++)pthread_create(&tid[i], NULL, thread_func, nizovi[i]);
    for(int i=0;i<n;i++)pthread_join(tid[i], NULL);
    pthread_mutex_destroy(&globalLock);
    printf("%d\n", res);
    free(tid);
    for(int i=0;i<n;i++)
    {
        free(nizovi[i]);
        free(mat[i]);
    }
    free(nizovi);
    free(mat);
    exit(EXIT_SUCCESS);
}