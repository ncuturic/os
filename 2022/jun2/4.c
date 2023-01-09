#define _POSIX_C_SOURCE 2
#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
#define MAXS 1024
typedef struct dt
{
    int fd, i;
}epoll_data;
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    unsigned numFifos = argc-1;
    int epollFd = epoll_create(numFifos);
    check_error(epollFd!=-1, "epoll_create");
    int* cnt = (int*)calloc(numFifos, sizeof(int));
    check_error(cnt!=NULL, "malloc");
    epoll_data* data = (epoll_data*)malloc(numFifos*sizeof(epoll_data));
    check_error(data!=NULL, "malloc");
    for(unsigned i=0;i<numFifos;i++)
    {
        int fd = open(argv[i+1], O_RDONLY);
        check_error(fd!=-1, "open");
        struct epoll_event* event = (struct epoll_event*)malloc(sizeof(struct epoll_event));
        check_error(event!=NULL, "malloc");
        event->events = EPOLLIN;
        data[i].fd = fd;
        data[i].i = i;
        event->data.ptr = &data[i];
        check_error(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, event)!=-1, "epoll_ctl");
    }
    struct epoll_event* readyEvents = (struct epoll_event*)malloc(numFifos*sizeof(struct epoll_event));
    check_error(readyEvents!=NULL, "malloc");
    while(numFifos)
    {
        int readyFifos = epoll_wait(epollFd, readyEvents, numFifos, -1);
        check_error(readyFifos!=-1, "epoll_wait");
        for(int i=0;i<readyFifos;i++)
        {
            if(readyEvents[i].events & EPOLLIN)
            {
                epoll_data* event = (epoll_data*)readyEvents[i].data.ptr;
                char* buff = (char*)malloc(MAXS);
                check_error(buff!=NULL, "malloc");
                check_error(read(event->fd, buff, MAXS)!=-1, "read");
                char* c;
                while((c = strchr(buff, ' '))!=NULL)
                {
                    cnt[event->i]++;
                    buff = c+1;
                }
                if(buff!=NULL && (*buff!=0 || *buff!=' '))cnt[event->i]++;
            }
            else if(readyEvents[i].events & (EPOLLHUP | EPOLLERR))
            {
                numFifos--;
                epoll_data* event = (epoll_data*)readyEvents[i].data.ptr;
                close(event->fd);
            }
        }
    }
    numFifos = argc-1;
    int max = 0;
    int maxIdx = -1;
    for(unsigned i=0;i<numFifos;i++)
    {
        if(cnt[i]>max)
        {
            max = cnt[i];
            maxIdx = i;
        }
    }
    char* c = strrchr(argv[maxIdx+1], '/');
    if(c==NULL)printf("%s %d\n", argv[maxIdx+1], max);
    else printf("%s %d\n", c+1, max);
    free(cnt);
    free(data);
    free(readyEvents);
    exit(EXIT_SUCCESS);
}