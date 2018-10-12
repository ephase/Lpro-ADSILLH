#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PAUSE 60
#define CHILDREN 2
int pids[CHILDREN];

void signal_sigterm () {
    for(int i=0; i<CHILDREN; i++){
        // do nothing if whe are in childs
        if (pids[i] > 0) {
            if ((kill(pids[i],15)) == -1){
                perror("Error in signal catch\n");
            }
            printf("Sending SIGTERM to child %d\n", pids[i]);
        }
        else{
            printf("\tKilling me\n");
            exit(EXIT_SUCCESS);
        }
    }
}

int main( int argc, char **argv){
       printf("Starting father with pid %d\n", getpid());
    int status;
    for (int i=0;i<CHILDREN;i++){
        pids[i] = fork();
        if (pids[i] == 0) {
            sleep(PAUSE);
            exit(EXIT_SUCCESS);
        }
        else {
            printf ("Starting child with pid : %d\n", pids[i]);
        }
    }
    if((signal(SIGTERM,signal_sigterm)) == SIG_ERR){
        perror("unable to catch signal");
    }
    waitpid(-1, &status,0);
    exit(EXIT_SUCCESS);
}
