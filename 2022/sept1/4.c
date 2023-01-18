#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<string.h>
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
#define MAXS 256
char* globalString;
pthread_mutex_t globalLock;
void* thread_func(void* arg)
{
    char* s = (char*)arg;
    int len = strlen(s);
    char* rez = (char*)malloc(len+1);
    check_error(rez!=NULL, "malloc");
    for(int i=0;i<len;i++)rez[i] = s[len-1-i];
    rez[len] = 0;
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    strcat(globalString, rez);
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return rez;
}
int main(int argc, char* argv[])
{
    int n;
    scanf("%d", &n);
    check_error(n>0, "broj stringova");
    char** strings = (char**)malloc(n*sizeof(char*));
    check_error(strings!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        strings[i] = (char*)malloc(MAXS);
        check_error(strings[i]!=NULL, "malloc");
        scanf("%s", strings[i]);
    }
    globalString = (char*)calloc(n*MAXS, sizeof(char));
    check_error(globalString!=NULL, "calloc");
    pthread_t* tids = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(tids!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<n;i++)thread_error(pthread_create(&tids[i], NULL, thread_func, strings[i]), "pthread_create");
    for(int i=0;i<n;i++)
    {
        void* rez;
        thread_error(pthread_join(tids[i], &rez), "pthread_join");
        printf("%s\n", (char*)rez);
        free(rez);
    }
    printf("%ld\n", strlen(globalString));
    free(tids);
    for(int i=0;i<n;i++)free(strings[i]);
    free(strings);
    free(globalString);
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    exit(EXIT_SUCCESS);
}