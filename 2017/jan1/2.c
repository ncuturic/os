#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond))\
        {\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 256
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    int pipeFds[2];
    check_error(pipe(pipeFds)!=-1, "pipe");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        close(pipeFds[0]);
        close(STDOUT_FILENO);
        check_error(dup2(pipeFds[1], STDOUT_FILENO)!=-1, "dup");
        check_error(execlp("stat", "stat", argv[1], NULL)!=-1, "exec");
    }
    else
    {
        close(pipeFds[1]);
        int status = 0;
        check_error(waitpid(pid, &status, 0)!=-1, "wait");
        if(WIFEXITED(status) && WEXITSTATUS(status)==0)
        {
            FILE* ulaz = fdopen(pipeFds[0], "r");
            char s[MAXS];
            while(fscanf(ulaz, "%s", s)!=EOF)
            {
                if(!strcmp(s, "Size:"))
                {
                    size_t size;
                    fscanf(ulaz, "%ld", &size);
                    printf("%ld\n", size);
                    break;
                }
            }
            fclose(ulaz);
        }
        else printf("Neuspeh\n");
        close(pipeFds[0]);
    }
    exit(EXIT_SUCCESS);
}