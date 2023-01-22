#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
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
#define MAXS 1024
int main(int argc, char* argv[])
{
	check_error(argc==2, "argumenti");
	int pipeFds[2];
	check_error(pipe(pipeFds)!=-1, "pipe");
	pid_t pid = fork();
	check_error(pid!=-1, "fork");
	if(pid==0)
	{
		close(STDOUT_FILENO);
		close(pipeFds[RD_END]);
		check_error(dup2(pipeFds[WR_END], STDOUT_FILENO)!=-1, "dup2");
		check_error(execlp("find", "find", ".", "-type", "f", NULL)!=-1, "execlp");
	}
	else
	{
		close(pipeFds[WR_END]);
		int status = 0;
		int cnt = 0;
		check_error(wait(&status)!=-1, "wait");
		check_error(WIFEXITED(status) && WEXITSTATUS(status)==0, "greska u detetu");
		FILE* stream = fdopen(pipeFds[RD_END], "r");
		check_error(stream!=NULL, "fdopen");
		char linija[MAXS];
		while(fgets(linija, MAXS, stream)!=NULL)
		{
			linija[strlen(linija)-1] = 0;
			char* ext = strrchr(linija, '.');
			if(ext!=NULL && strcmp(ext, argv[1])==0)cnt++;
		}
		fclose(stream);
		close(pipeFds[WR_END]);
		printf("%d\n", cnt);
	}
	exit(EXIT_SUCCESS);
}
