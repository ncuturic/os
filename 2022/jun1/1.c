#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define greska(cond, msg) do{\
        if(!(cond)){\
            fprintf(stderr, "%s\n", (msg));\
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
    greska(argc==3, "argumenti");
    time_t sekunde = atol(argv[1]);
    struct tm *vreme = localtime(&sekunde);
    greska(vreme!=NULL, "localtime");
    printf("%s ", mesec(vreme->tm_mon));
    (vreme->tm_mon)-=atoi(argv[2]);
    mktime(vreme);
    printf("%s\n", mesec(vreme->tm_mon));
    exit(EXIT_SUCCESS);
}
