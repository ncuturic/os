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
        int __err = (func);\
        if(__err){\
            errno = __err;\
            check_error(0, msg);\
        }\
    }while(0)
int sum;
pthread_mutex_t globalLock;
void* threadFunc(void* arg)
{
    int broj = *((int*)arg);
    int broj1 = broj;
    int brCifara = 0;
    int prva = 1;
    do
    {
        brCifara++;
        broj1/=10;
        prva *= 10;
    }while(broj1);
    prva /= 10;
    broj = (broj%10)*prva + (broj%prva) - (broj%10) + broj/prva;
    
    *((int*)arg) = broj;
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    sum += broj;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return NULL;
}
int main(int argc, char* argv[])
{
    int n;
    scanf("%d", &n);
    int* niz = (int*)malloc(n*sizeof(int));
    check_error(niz!=NULL, "malloc");
    for(int i=0;i<n;i++)scanf("%d", &niz[i]);

    pthread_t* tids = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(tids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<n;i++)thread_error(pthread_create(&tids[i], NULL, threadFunc, &niz[i]), "pthread_create");
    for(int i=0;i<n;i++)thread_error(pthread_join(tids[i], NULL), "pthread_join");
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    
    for(int i=0;i<n;i++)printf("%d ", niz[i]);
    printf("\n%d\n", sum);
    free(tids);
    free(niz);
    exit(EXIT_SUCCESS);
}