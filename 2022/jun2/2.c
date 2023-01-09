#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 256
#define RD_END 0
#define WR_END 1
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    FILE* ulaz = fopen(argv[1], "r");
    char* linija = (char*)malloc(20*MAXS);
    check_error(linija!=NULL, "malloc");
    unsigned max = 0;
    char* res = NULL;
    while(fgets(linija, 20*MAXS, ulaz)!=NULL)
    {
        int br = 0;
        char* argumenti[20];
        for(int i=0;i<20;i++)
        {
            argumenti[i] = (char*)malloc(MAXS);
            check_error(argumenti[i]!=NULL, "malloc");
        }
        while(sscanf(linija, "%s", argumenti[br++])!=EOF)
        {
            char* c = strchr(linija, ' ');
            if(c==NULL)break;
            linija = c+1;
        }
        argumenti[br] = NULL;
        int pipeFds[2];
        check_error(pipe(pipeFds)!=-1, "pipe");
        pid_t pid = fork();
        check_error(pid!=-1, "fork");
        if(pid==0)
        {
            close(pipeFds[RD_END]);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            check_error(dup2(open("/dev/null", O_WRONLY), STDOUT_FILENO)!=-1, "dup2");
            check_error(dup2(pipeFds[WR_END], STDERR_FILENO)!=-1, "dup2");
            check_error(execvp(argumenti[0], argumenti)!=-1, "exec");
        }
        else
        {
            close(pipeFds[WR_END]);
            int status = 0;
            check_error(waitpid(pid, &status, 0)!=-1, "waitpid");
            if(WIFEXITED(status) && WEXITSTATUS(status))
            {
                unsigned read_bytes = 0;
                unsigned total_bytes = 0;
                char buff[MAXS];
                while((read_bytes = read(pipeFds[RD_END], buff, MAXS)) > 0)total_bytes += read_bytes;
                if(read_bytes == -1)check_error(0, "read");
                if(total_bytes > max)
                {
                    max = total_bytes;
                    res = argumenti[0];
                }
            }
        }
        //for(int i=0;i<20;i++)free(argumenti[i]);
    }
    //free(linija);
    if(res == NULL)printf("unknown 0\n");
    else printf("%s %u\n", res, max);
    fclose(ulaz);
    exit(EXIT_SUCCESS);
}