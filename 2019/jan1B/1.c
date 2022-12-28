#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* mesec(int broj)
{
    if(broj==0)return "januar";
    if(broj==1)return "februar";
    if(broj==2)return "mart";
    if(broj==3)return "april";
    if(broj==4)return "maj";
    if(broj==5)return "jun";
    if(broj==6)return "jul";
    if(broj==7)return "avgust";
    if(broj==8)return "septembar";
    if(broj==9)return "oktobar";
    if(broj==10)return "novembar";
    if(broj==11)return "decembar";
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==3, "argumenti");
    time_t current_time = atol(argv[1]);
    struct tm* broken_time = localtime(&current_time);
    printf("%s ", mesec(broken_time->tm_mon));
    broken_time->tm_mday += atoi(argv[2]);
    check_error(mktime(broken_time)!=-1, "mktime");
    printf("%s\n", mesec(broken_time->tm_mon));
    exit(EXIT_SUCCESS);
}
