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
    check_error(argc==2, "./a.out file");
    FILE* komande = fopen(argv[1], "r");
    check_error(komande!=NULL, "fopen");
    char* komanda = (char*)malloc(MAXS);
    check_error(komanda!=NULL, "malloc");
    char* res = (char*)malloc(MAXS);
    check_error(res!=NULL, "malloc");
    int maks = -1;
    while(fgets(komanda, MAXS, komande))
    {
        int fds[2];
        check_error(pipe(fds)!=-1, "pipe");
        pid_t pid;
        check_error((pid=fork())!=-1, "fork");
        if(pid==0)
        {
            close(fds[RD_END]);
            close(STDOUT_FILENO);
            check_error(dup2(fds[WR_END], STDOUT_FILENO)!=-1, "dup");
            char *pom = strchr(komanda, ' ');
            check_error(pom!=NULL, "strchr");
            int idx = pom - komanda;
            char* arg1 = (char*)malloc(idx+1);
            check_error(arg1!=NULL, "malloc");
            for(int i=0;i<idx;i++)arg1[i]=komanda[i];
            arg1[idx]=0;
            int n = strlen(komanda);
            char* arg2 = (char*)malloc(n - idx);
            check_error(arg2!=NULL, "malloc");
            for(int i = idx+1; i<n-1; i++)arg2[i-idx-1] = komanda[i];
            arg2[n-idx-2] = 0;
            char *const args[] = {arg1, arg2, NULL};
            check_error(execvp(arg1, args)!=-1, "exec");
        }
        else
        {
            close(fds[WR_END]);
            int status = 0;
            check_error(waitpid(pid, &status, 0)!=-1, "wait");
            check_error(WIFEXITED(status), "greska u detetu");
            ssize_t read_bytes, total_read=0;
            char* buffer = (char*)malloc(BUFF_SIZE);
            check_error(buffer!=NULL, "malloc");
            while((read_bytes = read(fds[RD_END], buffer, BUFF_SIZE)) > 0)
            {
                total_read += read_bytes;
            }
            check_error(read_bytes==0, "read");
            if(total_read > maks)
            {
                maks = total_read;
                strcpy(res, komanda);
            }
            close(fds[RD_END]);
            free(buffer);
        }
    }
    printf("%s", res);
    free(res);
    free(komanda);
    fclose(komande);
    exit(EXIT_SUCCESS);
}
