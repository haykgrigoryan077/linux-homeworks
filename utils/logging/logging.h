#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include "string.h"
#include <vector>



int System(int argc, char** argv) {
    int file = open(argv[1] , O_RDWR );
    char buf[] = "grel";
    int child = fork();
    if ( child == 0) {
        write(file,buf,sizeof(buf) - 1);
        exit(EXIT_SUCCESS);
    } else {
        int status;
        waitpid(child, &status, 0);
        ssize_t r;
        char buf2[sizeof(buf)+1];
        r = read(file, buf2, sizeof(buf2));
        if (r < 0)
        {
            perror(nullptr);
            exit(EXIT_FAILURE);
        }
        buf2[sizeof(buf)] = '\0';
        std::cout << buf2;
    }
    close(file);

}

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

void log_fatal(int error){
    std::cerr << strerror(error);
    exit(EXIT_FAILURE);
}

int CP(int argc, char** argv)
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
    int bytesRead = read(file1, file1Read.data(), sizeof(BytesToRead));
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

}

int hacker(int argc, char** argv) {

    const int initialFileDescriptor = open(argv[1], O_WRONLY);

    if (initialFileDescriptor == -1) {
        log_fatal(errno);
    }

    int size = fsize(argv[1]);
    int chunk_count = size / 4096;

    for(int i = 0; i < chunk_count; i++) {
        vector<char> ch(4096, '\0');
        write(initialFileDescriptor, ch.data(), ch.size());
    }

    close(initialFileDescriptor);
    unlink(argv[1]);
    if (unlink(argv[1]) == -1) {
        log_fatal(errno);
    }

}




