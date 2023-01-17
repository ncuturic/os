#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#define MAXS 4096
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
typedef struct epolldata
{
    int fd, cnt;
    char* naziv;
}Data;
int main(int argc, char* argv[])
{
    check_error(argc>1, "argumenti");
    int numFifos = argc-1;
    int epollFd = epoll_create(numFifos);
    check_error(epollFd!=-1, "epoll_create");
    struct epoll_event currentEvent;
    for(int i=0;i<numFifos;i++)
    {
        int fd = open(argv[i+1], O_RDONLY | O_NONBLOCK);
        check_error(fd!=-1, "open");
        currentEvent.events = EPOLLIN;
        Data* data = (Data*)malloc(sizeof(Data));
        check_error(data!=NULL, "malloc");
        data->fd = fd;
        data->cnt = 0;
        char* c = strrchr(argv[i+1], '/');
        if(c==NULL)data->naziv = argv[i+1];
        else data->naziv = c+1;
        currentEvent.data.ptr = data;
        check_error(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &currentEvent)!=-1, "epoll_ctl");
    }
    int activeFifos = numFifos;
    struct epoll_event* readyFifos = (struct epoll_event*)malloc(numFifos*sizeof(struct epoll_event));
    char* res;
    int max = 0;
    while(activeFifos)
    {
        int actions = epoll_wait(epollFd, readyFifos, numFifos, -1);
        check_error(actions!=-1, "epoll_wait");
        for(int i=0;i<actions;i++)
        {
            if(readyFifos[i].events & EPOLLIN)
            {
                char buff[MAXS];
                ssize_t read_bytes = 0;
                check_error((read_bytes = read(((Data*)readyFifos[i].data.ptr)->fd, buff, MAXS))!=-1, "read");
                buff[read_bytes] = 0;
                for(int j=0;j<read_bytes;j++)
                {
                    if(isdigit(buff[j]))((Data*)readyFifos[i].data.ptr)->cnt++;
                }
            }
            else if(readyFifos[i].events & (EPOLLHUP | EPOLLERR))
            {
                if(((Data*)readyFifos[i].data.ptr)->cnt > max)
                {
                    max = ((Data*)readyFifos[i].data.ptr)->cnt;
                    res = ((Data*)readyFifos[i].data.ptr)->naziv;
                }
                close(((Data*)readyFifos[i].data.ptr)->fd);
                free(readyFifos[i].data.ptr);
                activeFifos--;
            }
        }
    }
    printf("%s %d\n", res, max);
    exit(EXIT_FAILURE);
}