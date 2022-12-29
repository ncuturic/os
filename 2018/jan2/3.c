#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define RD_END 0
#define WR_END 1
#define MAXS 4096
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    int pipeFd[2];
    check_error(pipe(pipeFd)!=-1, "pipe");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        check_error(close(pipeFd[RD_END])!=-1, "close");
        check_error(close(STDOUT_FILENO)!=-1, "close");
        check_error(dup2(pipeFd[WR_END], STDOUT_FILENO)!=-1, "dup");
        check_error(execlp("tail", "tail", "-n", argv[2], argv[1], NULL)!=-1, "exec");
    }
    else
    {
        check_error(close(pipeFd[WR_END])!=-1, "close");
        int status = 0;
        check_error(waitpid(pid, &status, 0)!=-1, "wait");
        if(WIFEXITED(status) && WEXITSTATUS(status)==0)
        {
            FILE* fajl = fdopen(pipeFd[RD_END], "r");
            check_error(fajl!=NULL, "fdopen");
            char linija[MAXS];
            while(fgets(linija, MAXS, fajl)!=NULL)printf("%s", linija);
            fclose(fajl);
        }
        else printf("Neuspeh\n");
        close(pipeFd[RD_END]);
    }
    exit(EXIT_SUCCESS);
}