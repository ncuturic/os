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
int sum;
int maks = INT_MIN;
char* res;
pthread_mutex_t globalLock;
void* thread_function(void* arg)
{
    FILE* f = fopen((char*)arg, "r");
    int cnt = 0;
    char buff[MAXS];
    while(fscanf(f, "%s", buff)!=EOF)cnt++;
    fclose(f);
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    sum += cnt;
    if(cnt > maks)
    {
        maks = cnt;
        res = (char*)arg;
    }
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    int n = argc-1;
    pthread_t* pids = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(pids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i = 0;i<n;i++)
    {
        thread_error(pthread_create(&pids[i], NULL, thread_function, argv[i+1]), "pthread_create");
    }
    for(int i = 0; i<n; i++)
    {
        thread_error(pthread_join(pids[i], NULL), "pthread_join");
    }
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    free(pids);
    printf("%d %s\n", sum, res);
    exit(EXIT_SUCCESS);
}