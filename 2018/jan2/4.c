#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    int n = argc-1;
    int epollFd = epoll_create(n);
    check_error(epollFd!=-1, "epoll_create");
    struct epoll_event current_event;
    for(int i=0;i<n;i++)
    {
        memset(&current_event, 0, sizeof(current_event));
        current_event.data.fd = open(argv[i+1], O_RDONLY);
        check_error(current_event.data.fd!=-1, "open");
        current_event.events = EPOLLIN;
        check_error(epoll_ctl(epollFd, EPOLL_CTL_ADD, current_event.data.fd, &current_event)!=-1, "epoll_ctl");
    }
    int openFiles = n;
    struct epoll_event* readyFds = (struct epoll_event*)malloc(n*sizeof(struct epoll_event));
    check_error(readyFds!=NULL, "malloc");
    while(openFiles)
    {
        int filesReady = 0;
        check_error((filesReady=epoll_wait(epollFd, readyFds, n, -1))!=-1, "epoll_wait");
        for(int i=0;i<filesReady;i++)
        {
            if(readyFds[i].events & EPOLLIN)
            {
                
            }
            else if(readyFds[i].events & (EPOLLHUP | EPOLLERR))
            {
                close(readyFds[i].data.fd);
                openFiles--;
            }
        }
    }
    close(epollFd);
    exit(EXIT_SUCCESS);
}