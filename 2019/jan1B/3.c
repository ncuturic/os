//Ne radi
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
        int tmp = (func);\
        if(tmp!=0){\
            errno = tmp;\
            check_error(false, msg);\
        }\
    }while(0)
typedef struct td
{
    int n, size;
    float** vektori;
}ThreadData;
void* thread_func(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    float *local_max = (float*)malloc(sizeof(float));
    *local_max = 0;
    for(int i=0;i<data->size;i++)*local_max += data->vektori[0][i] * data->vektori[data->n][i];
    for(int i=1;i<data->n;i++)
    {
        float skalarni = 0;
        for(int j=0;j<data->size;j++)skalarni += data->vektori[i][j] * data->vektori[data->n][j];
        if(skalarni>*local_max)*local_max=skalarni;
    }
    return (void*)local_max;
}
int main()
{
    int m, n, k;
    scanf("%d%d%d", &m, &n, &k);
    float** vektori = (float**)malloc((m+1)*sizeof(float*));
    check_error(vektori!=NULL, "malloc");
    for(int i=0;i<m+1;i++)
    {
        vektori[i] = (float*)malloc(n*sizeof(float));
        check_error(vektori[i]!=NULL, "malloc");
        for(int j=0;j<n;j++)scanf("%f", &vektori[i][j]);
    }
    pthread_t* tid = (pthread_t*)malloc(k*sizeof(pthread_t));
    check_error(tid!=NULL, "malloc");
    ThreadData* data = (ThreadData*)malloc(k*sizeof(ThreadData));
    check_error(data!=NULL, "malloc");
    for(int i=0;i<k;i++)
    {
        data[i].vektori = (float**)malloc(((m/k)+1)*sizeof(float*));
        for(int j = i*m/k; j<(i+1)*m/k; j++)data[i].vektori[j-i*m/k] = vektori[j];
        data[i].vektori[m/k] = vektori[m];
        data[i].n = m/k;
        data[i].size = n;
    }
    for(int i=0;i<k;i++)pthread_create(&tid[i], NULL, thread_func, &data[i]);
    float max;
    void* t;
    int idx = 0;
    for(int i=0;i<k;i++)
    {
        pthread_join(tid[i], &t);
        //printf("%d %f\n", i, *(float*)t);
        if(i==0)
        {
            max = *(float*)t;
            idx = 0;
        }
        else if(*(float*)t>max)
        {
            max=*(float*)t;
            idx = i;
        }
    }
    printf("%d %f\n", idx, max);
    exit(EXIT_SUCCESS);
}