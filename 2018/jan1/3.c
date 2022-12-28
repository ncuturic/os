#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#define RD_END 0
#define WR_END 1
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
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
        check_error(dup2(pipeFds[WR_END], STDOUT_FILENO)!=-1, "dup");
        check_error(execlp("wc", "wc", argv[2], argv[1], NULL)!=-1, "exec");
    }
    else
    {
        close(pipeFds[WR_END]);
        int status = 0;
        check_error(waitpid(pid, &status, 0)!=-1, "wait");
        if(WIFEXITED(status) && WEXITSTATUS(status)==0)
        {
            FILE* ulaz = fdopen(pipeFds[RD_END], "r");
            check_error(ulaz!=NULL, "fdopen");
            int br;
            fscanf(ulaz, "%d", &br);
            printf("%d\n", br);
            fclose(ulaz);
        }
        else printf("Neuspeh\n");
        close(pipeFds[RD_END]);
    }
    exit(EXIT_SUCCESS);
}