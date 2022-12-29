#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
char* dan(int x)
{
    if(x==0)return "nedelja";
    if(x==1)return "ponedeljak";
    if(x==2)return "utorak";
    if(x==3)return "sreda";
    if(x==4)return "cetvrtak";
    if(x==5)return "petak";
    if(x==6)return "subota";
    return NULL;
}
int main(int argc, char* argv[])
{
    check_error(argc==2, "argumenti");
    time_t sekunde = atol(argv[1]);
    struct tm* brokenTime = localtime(&sekunde);
    printf("%s\n", dan(brokenTime->tm_wday));
    exit(EXIT_SUCCESS);
}