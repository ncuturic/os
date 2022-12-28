#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int skor, term;
void sighandler(int signum)
{
    if(signum==SIGUSR1)skor++;
    if(signum==SIGUSR2)skor+=2;
    if(signum==SIGINT)skor-=4;
    if(signum==SIGTERM)term=1;
}
int main(int argc, char* argv[])
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGUSR2, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGINT, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGTERM, sighandler)!=SIG_ERR, "signal");
    while(!term)
    {
        pause();
    }
    printf("%d\n", skor);
    exit(EXIT_SUCCESS);
}
