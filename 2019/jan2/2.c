//Radi samo 20%
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define RD_END 0
#define WR_END 1
#define BUFF_SIZE 4096
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    for(int i=1;i<argc;i++)
    {
        int fds[2];
        check_error(pipe(fds)!=-1, "pipe");
        pid_t pid = fork();
        check_error(pid!=-1, "fork");
        if(pid==0)
        {
            close(fds[RD_END]);
            close(STDOUT_FILENO);
            check_error(dup2(fds[WR_END], STDOUT_FILENO)!=-1, "dup");
            char *const args[] = {"du", "-sch", argv[i]};
            check_error(execvp(args[0], args)!=-1, "exec");
        }
        else
        {
            close(fds[WR_END]);
            int status = 0;
            check_error(waitpid(pid, &status, 0)!=-1, "wait");
            if(!WIFEXITED(status) || WEXITSTATUS(status))printf("neuspeh ");
            else
            {
                char buff[BUFF_SIZE];
                ssize_t read_bytes = 0;
                check_error((read_bytes = read(fds[RD_END], buff, BUFF_SIZE))!=-1, "read");
                char rez[BUFF_SIZE];
                if(read_bytes==0)printf("neuspeh ");
                else
                {
                    sscanf(buff, "%s", rez);
                    printf("%s ", rez);
                }
            }
            close(fds[RD_END]);
        }
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}
