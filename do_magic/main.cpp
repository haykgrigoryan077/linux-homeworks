#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"


void log_fatal(int error){
    std::cerr << strerror(error);
    exit(EXIT_FAILURE);
}

void do_magic()
{
    int inFD = open("input.txt", O_RDONLY);
    if(inFD < 0)
    {
        log_fatal(errno);
    }

    int successDuplicated = dup2(inFD, 0);
    if (successDuplicated < 0)
    {
        log_fatal(errno);
    }
}