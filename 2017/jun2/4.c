//Ne radi
#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<poll.h>
#include<limits.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    int numFifos = argc-1;
    struct pollfd* pollK = (struct pollfd*)calloc(numFifos, sizeof(struct pollfd));
    check_error(pollK!=NULL, "calloc");
    for(int i=0;i<numFifos;i++)
    {
        pollK[i].fd = open(argv[i+1], O_RDONLY | O_NONBLOCK);
        check_error(pollK[i].fd!=-1, "open");
        pollK[i].events = POLLIN;
    }
    int activeFifos = numFifos;
    int max = INT_MIN;
    int idx = 0;
    while(activeFifos)
    {
        printf("%d\n", activeFifos);
        check_error(poll(pollK, numFifos, -1)!=-1, "poll");
        for(int i=0;i<numFifos;i++)
        {
            if(pollK[i].revents & POLLIN)
            {
                FILE* ulaz = fdopen(pollK[i].fd, "r");
                check_error(ulaz!=NULL, "fdopen");
                int x=0;
                while(fscanf(ulaz, "%d", &x)!=EOF)
                {
                    printf("%d ", x);
                    if(x>max)
                    {
                        max = x;
                        idx = i;
                    }
                }
                printf("\n");
                fclose(ulaz);
                pollK[i].revents = 0;
            }
            else if(pollK[i].revents & (POLLHUP | POLLERR))
            {
                activeFifos--;
                close(pollK[i].fd);
                pollK[i].revents = 0;
            }
        }
    }
    char* res = strrchr(argv[idx+1], '/');
    if(res==NULL)res = argv[idx+1];
    else res++;
    printf("%d %s\n", max, res);
    free(pollK);
    exit(EXIT_SUCCESS);
}
