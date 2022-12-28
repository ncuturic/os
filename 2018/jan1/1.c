#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* mesec(int x)
{
    if(x==0)return "januar";
    if(x==1)return "februar";
    if(x==2)return "mart";
    if(x==3)return "april";
    if(x==4)return "maj";
    if(x==5)return "jun";
    if(x==6)return "jul";
    if(x==7)return "avgust";
    if(x==8)return "septembar";
    if(x==9)return "oktobar";
    if(x==10)return "novembar";
    if(x==11)return "decembar";
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    time_t sekunde = atol(argv[1]);
    struct tm* brokenTime = localtime(&sekunde);
    check_error(brokenTime!=NULL, "localtime");
    printf("%s\n", mesec(brokenTime->tm_mon));
    exit(EXIT_SUCCESS);
}