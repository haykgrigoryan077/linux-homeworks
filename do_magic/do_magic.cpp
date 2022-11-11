#include <iostream>
#include <fcntl.h>
#include <unistd.h>

void do_magic()
{
    int inFD = open("input.txt", O_RDONLY);
    if(inFD < 0)
    {
        perror("open tozbishlar");
        exit(1);
    }

    int successDuplicated = dup2(inFD, 0);
    if (successDuplicated < 0)
    {
        perror("dup tozbishlar");
        exit(1);
    }
}