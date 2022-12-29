#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#define MAXS 64
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int sigusr1, sigusr2, sigterm;
void sighandler(int signum)
{
    if(signum==SIGUSR1)sigusr1 = 1;
    if(signum==SIGUSR2)sigusr2 = 1;
    if(signum==SIGTERM)sigterm = 1;
}
int main()
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGUSR2, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGTERM, sighandler)!=SIG_ERR, "signal");
    char s[MAXS];
    while(1)
    {
        scanf("%s", s);
        int n = strlen(s);
        pause();
        if(sigusr1)
        {
            for(int i=n-1;i>=0;i--)printf("%c", s[i]);
            printf("\n");
            sigusr1 = 0;
        }
        if(sigusr2)
        {
            for(int i=0;i<n;i++)printf("%c", toupper(s[i]));
            printf("\n");
            sigusr2 = 0;
        }
        if(sigterm)break;
    }
    exit(EXIT_SUCCESS);
}