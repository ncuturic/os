#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 4096
#define RD_END 0
#define WR_END 1
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int pipeFds[2];
    check_error(pipe(pipeFds)!=-1, "pipe");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        close(pipeFds[RD_END]);
        close(STDOUT_FILENO);
        check_error(dup2(pipeFds[WR_END], STDOUT_FILENO)!=-1, "dup2");
        check_error(execlp("cat", "cat", argv[1], NULL)!=-1, "execlp");
    }
    else
    {
        close(pipeFds[WR_END]);
        int status = 0;
        check_error(wait(&status)!=-1, "wait");
        check_error(WIFEXITED(status) && WEXITSTATUS(status)==0, "greska u detetu");
        char linija[MAXS];
        char najduza[MAXS];
        int brLinija;
        FILE* ulaz = fdopen(pipeFds[RD_END], "r");
        check_error(ulaz!=NULL, "fdopen");
        while(fgets(linija, MAXS, ulaz)!=NULL)
        {
            brLinija++;
            if(strlen(linija) > strlen(najduza))strcpy(najduza, linija);
        }
        printf("%d %ld\n", brLinija, strlen(najduza));
        fclose(ulaz);
        close(pipeFds[RD_END]);
    }
    exit(EXIT_SUCCESS);
}