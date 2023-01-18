#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
#include<pwd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 1024
int sigusr1, sigquit;
void sighandler(int signum)
{
    if(signum==SIGUSR1)sigusr1++;
    if(signum==SIGQUIT)sigquit++;
}
int main(int argc, char* argv[])
{
    check_error(signal(SIGUSR1, sighandler)!=SIG_ERR, "signal");
    check_error(signal(SIGQUIT, sighandler)!=SIG_ERR, "signal");
    char file[MAXS];
    while(1)
    {
        scanf("%s", file);
        pause();
        if(sigusr1)
        {
            sigusr1 = 0;
            struct stat fileinfo;
            check_error(lstat(file, &fileinfo)!=-1, "lstat");
            struct passwd* userinfo;
            check_error((userinfo=getpwuid(fileinfo.st_uid))!=NULL, "getpwuid");
            printf("%s\n", userinfo->pw_name);
        }
        else if(sigquit)break;
    }
    exit(EXIT_SUCCESS);
}