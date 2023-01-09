#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#define MAXS 1024
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int sigusr1, sigusr2, sigquit;
void sighandler(int signum)
{
    if(signum == SIGUSR1)sigusr1 = 1;
    if(signum == SIGUSR2)sigusr2 = 1;
    if(signum == SIGQUIT)sigquit = 1;
}
int palindrom(char* s)
{
    int i = 0;
    int j = strlen(s) - 1;
    while(i<=j)
    {
        if(s[i]!=s[j])return 0;
        i++;
        j--;
    }
    return 1;
}
int main(int argc, char* argv[])
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGUSR2, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGQUIT, sighandler)!=SIG_ERR, "signal");
    char s[MAXS];
    while(1)
    {
        scanf("%s", s);
        pause();
        if(sigquit)break;
        if(sigusr1)
        {
            sigusr1 = 0;
            printf("%d ", palindrom(s));
        }
        else if(sigusr2)
        {
            sigusr2 = 0;
            int n = strlen(s);
            for(int i = n-1; i>=0; i--)printf("%c", s[i]);
            printf(" ");
        }
    }
    exit(EXIT_SUCCESS);
}