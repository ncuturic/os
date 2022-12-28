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
    time_t current_time = atoi(argv[1]);
    struct tm* brokent_time = localtime(&current_time);
    if(0 <= brokent_time->tm_hour && brokent_time->tm_hour <= 6)printf("noc\n");
    else if(7 <= brokent_time->tm_hour && brokent_time->tm_hour <=8)printf("jutro\n");
    else if(9 <= brokent_time->tm_hour && brokent_time->tm_hour <=11)printf("prepodne\n");
    else if(12 <= brokent_time->tm_hour && brokent_time->tm_hour <=18)printf("popodne\n");
    else printf("vece\n");
    exit(EXIT_SUCCESS);
}
