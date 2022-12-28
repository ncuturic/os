#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
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
int main()
{
    int neuspesni = 0, maks = -1;
    char res[MAXS] = "nema";
    char* linija = (char*)malloc(MAXS);
    char* linija0 = linija;
    check_error(linija!=NULL, "malloc");
    while(fgets(linija, MAXS, stdin))
    {
        int n;
        sscanf(linija, "%d", &n);
        linija = strchr(linija, ' ') + 1;
        char** args = (char**)malloc((n+1)*sizeof(char*));
        check_error(args!=NULL, "malloc");
        for(int i=0;i<n;i++)
        {
            args[i] = (char*)malloc(MAXS);
            check_error(args[i]!=NULL, "malloc");
            sscanf(linija, "%s", args[i]);
            linija = strchr(linija, ' ') + 1;
        }
        args[n] = (char*)malloc(MAXS);
        check_error(args[n]!=NULL, "malloc");
        args[n]=NULL;
        int fds[2];
        check_error(pipe(fds)!=-1, "pipe");
        pid_t pid = fork();
        check_error(pid!=-1, "fork");
        if(pid==0)
        {
            close(fds[RD_END]);
            close(STDOUT_FILENO);
            check_error(dup2(fds[WR_END], STDOUT_FILENO)!=-1, "dup");
            check_error(execvp(args[0], args)!=-1, "exec");
        }
        else
        {
            close(fds[WR_END]);
            int status = 0;
            check_error(waitpid(pid, &status, 0)!=-1, "wait");
            if(!WIFEXITED(status) || WEXITSTATUS(status))neuspesni++;
            else
            {
                ssize_t read_bytes = 0, total_read = 0;
                char buff[BUFF_SIZE];
                while((read_bytes = read(fds[RD_END], buff, BUFF_SIZE))>0)
                {
                    total_read += read_bytes;
                }
                check_error(read_bytes!=-1, "read");
                if(total_read > maks)
                {
                    maks = total_read;
                    strcpy(res, args[0]);
                }
            }
            close(fds[RD_END]);
        }
        for(int i=0;i<=n;i++)free(args[i]);
        free(args);
        linija = linija0;
    }
    printf("%d %s\n", neuspesni, res);
    free(linija0);
    exit(EXIT_SUCCESS);
}
