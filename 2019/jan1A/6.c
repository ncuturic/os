#include<stdio.h>
#include<stdlib.h>
#define check_error(cond, msg) do{\
        if(!(cond)){\
            perror(msg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
int main(int argc, char* argv[])
{
    check_error(argc==4, "./a.out dir oldExt newExt");

    exit(EXIT_SUCCESS);
}
