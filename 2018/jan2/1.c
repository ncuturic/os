#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    time_t sekunde = atol(argv[1]);
    struct tm* broken_time = localtime(&sekunde);
    check_error(broken_time!=NULL, "localtime");
    if(broken_time->tm_hour < 10)printf("0%d:", broken_time->tm_hour);
    else printf("%d:", broken_time->tm_hour);
    if(broken_time->tm_min < 10)printf("0%d\n", broken_time->tm_min);
    else printf("%d\n", broken_time->tm_min);
    exit(EXIT_SUCCESS);
}