#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#define MAX_LINE 256
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    check_error(strcmp(strrchr(argv[1], '.'), ".c")==0, "nije c fajl");
    pid_t pid = fork();
    check_error(pid!=-1, "fork");
    if(pid==0)
    {
        char *const args[] = {"gcc", "-std=c99", "-S", argv[1], "-o", "asm.s", NULL};
        check_error(execvp(args[0], args)!=-1, "exec");
    }
    else
    {
        int status = 0;
        check_error(waitpid(pid, &status, 0)!=-1, "wait");
        check_error(WIFEXITED(status), "greska u detetu");
        if(WEXITSTATUS(status))exit(WEXITSTATUS(status));
        int cnt = 0;
        FILE* ulaz = fopen("asm.s", "r");
        check_error(ulaz!=NULL, "fopen");
        char linija[MAX_LINE];
        char s[MAX_LINE];
        while(fgets(linija, MAX_LINE, ulaz))
        {
            sscanf(linija, "%s", s);
            if(strstr(s, "mov")!=NULL)cnt++;
        }
        fclose(ulaz);
        check_error(unlink("asm.s")!=-1, "unlink");
        printf("%d\n", cnt);
    }
    exit(EXIT_SUCCESS);
}
