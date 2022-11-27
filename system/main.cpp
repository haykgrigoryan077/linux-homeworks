#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char** argv) {
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
    return 0;
}
