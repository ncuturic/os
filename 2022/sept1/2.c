#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
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
    int pipeFds[2];
    check_error(pipe(pipeFds)!=-1, "pipe");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        close(pipeFds[RD_END]);
        close(STDOUT_FILENO);
        check_error(dup2(pipeFds[WR_END], STDOUT_FILENO)!=-1, "dup2");
        check_error(execlp("head", "head", "-n", argv[2], argv[1], NULL)!=-1, "execlp");
    }
    else
    {
        close(pipeFds[WR_END]);
        int status = 0;
        check_error(wait(&status)!=-1, "wait");
        check_error(WIFEXITED(status) && WEXITSTATUS(status)==0, "greska u detetu");
        FILE* ulaz = fdopen(pipeFds[RD_END], "r");
        char linija[MAXS];
        char res[MAXS];
        int max = 0;
        while(fgets(linija, MAXS, ulaz)!=NULL)
        {
            if(strlen(linija) > max)
            {
                max = strlen(linija);
                strcpy(res, linija);
            }
        }
        printf("%s", res);
        fclose(ulaz);
        close(pipeFds[RD_END]);
    }
    exit(EXIT_SUCCESS);
}