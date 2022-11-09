#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include <vector>

void log_fatal(int error){
    std::cerr << strerror(error);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    if(argc != 3) {
        log_fatal(errno);
    }
    int file1 = open(argv[1], O_RDONLY);
    for(int i = 0; i < argc; i++){
        std::cout << argv[i] <<std::endl;
    }
    if(file1 == -1) {
        log_fatal(errno);
    }
    std::string file1Read;
    char *BytesToRead[4096];
    int bytesRead = read(file1, file1Read.data(), BytesToRead.size());
    int file2 = open(argv[2], O_RDWR );
    if(file2 < 0){
        log_fatal(errno);
    }

    auto readBytes = read(file1, 0,0);

    int err = write(file2, file1Read.data(),bytesRead);
    if(err < 0){
        log_fatal(errno);
    }

    while (readBytes = read(file1, BytesToRead,sizeof(readBytes))) {
        write(file2,BytesToRead,readBytes);
    }

    close(file1);
    close(file2);
    return 0;
}
