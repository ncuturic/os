#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/time.h>
#include<time.h>
#include<ftw.h>
#include<signal.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int sigusr1, sigusr2, sigterm;
void sighandler(int signum)
{
    if(signum==SIGUSR1)sigusr1=1;
    else if(signum==SIGUSR2)sigusr2=1;
    else if(signum==SIGTERM)sigterm=1;
}
int main(int argc, char* argv[])
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGUSR2, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGTERM, sighandler)!=SIG_ERR, "signal");
    int x;
    do
    {
        scanf("%d", &x);
        pause();
        if(sigusr1)
        {
            printf("%d ", x<0 ? -x : x);
            sigusr1=0;
        }
        else if(sigusr2)
        {
            printf("%d ", x*x);
            sigusr2=0;
        }
    }while(!sigterm);
    exit(EXIT_SUCCESS);
}
