#include <sys/wait.h>
#include <unistd.h>

int do_command(char** programs){
    int waitChild;
    if ( fork() == 0 ) {
        execvp(programs[0], programs);
        return 1;
    }
    else {
        wait(&waitChild);
        return 0;
    }
}

