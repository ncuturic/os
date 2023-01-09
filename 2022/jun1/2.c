#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<limits.h>
#include<sys/stat.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define RD_END 0
#define WR_END 1
#define MAXS 1024
int main(int argc, char* argv[])
{
    int pipeFds[2];
    check_error(pipe(pipeFds)!=-1, "pipe");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        close(pipeFds[RD_END]);
        close(STDOUT_FILENO);
        check_error(dup2(pipeFds[WR_END], STDOUT_FILENO)!=-1, "dup2");
        check_error(execlp("pwd", "pwd", NULL)!=-1, "exec");
    }
    else
    {
        close(pipeFds[WR_END]);
        char buff[MAXS];
        int status = 0;
        check_error(waitpid(pid, &status, 0)!=-1, "wait");
        check_error(WIFEXITED(status) && WEXITSTATUS(status)==0, "greska u detetu");
        // char* realPath = realpath(".", NULL);
        // struct stat fileinfo;
        // check_error(stat(realPath, &fileinfo)!=-1, "stat");
        check_error(read(pipeFds[RD_END], buff, MAXS)!=-1, "read");
        struct stat fileinfo2;
        check_error(stat(buff, &fileinfo2)!=-1, "stat");
        while(strcmp(buff, "/"))
        {
            // if(fileinfo.st_uid==fileinfo2.st_uid)
            {
                printf("%s\n", strrchr(buff, '/')+1);
                break;
            }
        }
        // free(realPath);
    }
    exit(EXIT_SUCCESS);
}