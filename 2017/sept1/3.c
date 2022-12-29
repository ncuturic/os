//Nije dovrsen
#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<fcntl.h>
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
    struct epoll_event *fds = (struct epoll_event*)malloc(n*sizeof(struct epoll_event));
    check_error(fds!=NULL, "malloc");
    for(int i=0;i<n;i++)
    {
        memset(&fds[i], 0, sizeof(struct epoll_event));
        fds[i].events = EPOLLIN;
        fds[i].data.fd = open(argv[i+1], O_RDONLY | O_NONBLOCK);
        check_error(fds[i].data.fd, "open");
        check_error(epoll_ctl(epollFd, EPOLL_CTL_ADD, fds[i].data.fd, &fds[i])!=-1, "epoll_ctl");
    }
    double* sume = (double*)calloc(n, sizeof(double));
    while(n)
    {
        int cnt;
        check_error((cnt = epoll_wait(epollFd, fds, argc-1, -1))!=-1, "epoll_wait");
    }
    exit(EXIT_SUCCESS);
}