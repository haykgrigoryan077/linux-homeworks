#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace std;


void log_fatal(int error){
    std::cerr << strerror(error);
    exit(EXIT_FAILURE);
}

off_t fsize(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

int main(int argc, char** argv) {

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
    return 0;
}

