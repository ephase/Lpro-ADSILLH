#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
    pid_t pid;
    int status;
    pid = fork ();
    if ( pid < 0 ){
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ( pid == 0){
        printf(" Into child, command : %s\n",argv[1]);
        // we are in children
        int out = execvp(argv[1], &argv[1]);
        if ( out == -1 ){
            printf("Error : %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else {
        // if child exit code is not 0 then exit father with EXIT_FAILURE
        while ((pid = waitpid(pid, &status, 0)) > 0){
            (status == 0) ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
        };
    }
}
