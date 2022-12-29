#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int cnt1, cnt2;
void sighandler(int signum)
{
    if(signum==SIGUSR1)
    {
        cnt1++;
    }
    if(signum==SIGUSR2)
    {
        cnt2++;
    }
    if(signum==SIGTERM)
    {
        printf("%d %d\n", cnt1, cnt2);
        exit(EXIT_SUCCESS);
    }
}
int main(int argc, char* argv[])
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGUSR2, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGTERM, sighandler)!=SIG_ERR, "signal");
    while(1);
    exit(EXIT_SUCCESS);
}