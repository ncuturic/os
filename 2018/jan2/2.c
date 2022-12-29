//Ne radi
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<math.h>
#define ALLOC 100
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
typedef struct Par
{
    double x, y;
}par;
double minDist;
typedef struct ThreadData
{
    int i, n;
    par* niz;
}threadData;
pthread_mutex_t globalLock;
void* thread_func(void* arg)
{
    par* niz = ((threadData*)arg)->niz;
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    double localMin = minDist;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    for(int i=0;i<((threadData*)arg)->n;i++)
    {
        if(i!=((threadData*)arg)->i)
        {
            double dist = sqrt((niz[((threadData*)arg)->i].x-niz[i].x)*(niz[((threadData*)arg)->i].x-niz[i].x)+
            (niz[((threadData*)arg)->i].y-niz[i].y)*(niz[((threadData*)arg)->i].y-niz[i].y));
            if(dist<localMin)localMin = dist;
        }
    }
    thread_error(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
    if(localMin<minDist)minDist=localMin;
    thread_error(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
    return NULL;
}
int main(int argc, char* argv[])
{
    int n = 0;
    par* niz = (par*)malloc(ALLOC*sizeof(par));
    check_error(niz!=NULL, "malloc");
    int alocirano = ALLOC;
    double x, y;
    while(scanf("%lf%lf", &x, &y)!=EOF)
    {
        if(n==alocirano)
        {
            par* tmp = (par*)realloc(niz, alocirano+ALLOC);
            check_error(tmp!=NULL, "realloc");
            niz = tmp;
            alocirano += ALLOC;
        }
        niz[n].x = x;
        niz[n].y = y;
        n++;
    }
    minDist = sqrt((niz[0].x-niz[1].x)*(niz[0].x-niz[1].x)+(niz[0].y-niz[1].y)*(niz[0].y-niz[1].y));
    pthread_t* tid = (pthread_t*)malloc(n*sizeof(pthread_t));
    check_error(tid!=NULL, "malloc");
    threadData* data = (threadData*)malloc(n*sizeof(threadData));
    check_error(data!=NULL, "malloc");
    thread_error(pthread_mutex_init(&globalLock, NULL), "pthread_mutex_init");
    for(int i=0;i<n;i++)
    {
        data->i = i;
        data->n = n;
        data->niz = niz;
        thread_error(pthread_create(&tid[i], NULL, thread_func, &data[i]), "pthread_create");
    }
    for(int i=0;i<n;i++)thread_error(pthread_join(tid[i], NULL), "pthread_join");
    thread_error(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
    printf("%lf\n", minDist);
    free(data);
    free(tid);
    free(niz);
    exit(EXIT_SUCCESS);
}