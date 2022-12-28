#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#define BUF_SIZE 4096
void greska(char* poruka)
{
    fprintf(stderr, "%s\n", poruka);
    exit(EXIT_FAILURE);
}
int main(int argc, char* argv[])
{
    if(argc!=3)greska("losi argumenti");

    int fdSrc = open(argv[1], O_RDONLY);
    if(fdSrc==-1)greska("open src");
    int fdDest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if(fdDest==-1)greska("open dest");

    int r = 0;
    char* buf = malloc(BUF_SIZE);
    if(buf==NULL)greska("alokacija");
    while((r = read(fdSrc, buf, BUF_SIZE)) > 0)
    {
        if(write(fdDest, buf, r)!=r)greska("write");
    }
    if(r==-1)greska("read");
    free(buf);

    struct stat fileinfo;
    if(fstat(fdSrc, &fileinfo)==-1)greska("stat");
    if(fchmod(fdDest, fileinfo.st_mode)==-1)greska("chmod");

    close(fdSrc);
    close(fdDest);

    if(unlink(argv[1])==-1)greska("unlink");
    exit(EXIT_SUCCESS);
}
