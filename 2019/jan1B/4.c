#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#define MAXS 256
#define BUFF_SIZE 4096
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
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    check_error(ulaz!=NULL, "fopen");
    FILE* izlaz = fopen("errors.txt", "w");
    check_error(izlaz!=NULL, "fopen");
    char linija[MAXS];
    while(fgets(linija, MAXS, ulaz))
    {
        int fds[2];
        check_error(pipe(fds)!=-1, "pipe");
        pid_t pid = fork();
        check_error(pid!=-1, "fork");
        if(pid==0)
        {
            close(fds[RD_END]);
            close(STDERR_FILENO);
            check_error(dup2(fds[WR_END], STDERR_FILENO)!=-1, "dup");
            char *pom = strchr(linija, ' ');
            check_error(pom!=NULL, "strchr");
            int idx = pom - linija;
            char* arg1 = (char*)malloc(idx+1);
            check_error(arg1!=NULL, "malloc");
            for(int i=0;i<idx;i++)arg1[i]=linija[i];
            arg1[idx]=0;
            int n = strlen(linija);
            char* arg2 = (char*)malloc(n - idx);
            check_error(arg2!=NULL, "malloc");
            for(int i = idx+1; i<n-1; i++)arg2[i-idx-1] = linija[i];
            arg2[n-idx-2] = 0;
            char *const args[] = {arg1, arg2, NULL};
            check_error(execvp(arg1, args)!=-1, "exec");
        }
        else
        {
            close(fds[WR_END]);
            int status = 0;
            check_error(waitpid(pid, &status, 0)!=-1, "wait");
            if(!WIFEXITED(status) || WEXITSTATUS(status))
            {
                char buff[BUFF_SIZE];
                check_error(read(fds[RD_END], buff, BUFF_SIZE)!=-1, "read");
                fprintf(izlaz, "%s", buff);
            }
            close(fds[RD_END]);
        }
    }
    fclose(ulaz);
    fclose(izlaz);
    exit(EXIT_SUCCESS);
}
