#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int i;

void signal_sigint(){
    printf("value : %d\n",i);
    //exit(EXIT_SUCCESS);
}

int main () {
    if((signal(SIGINT,signal_sigint)) == SIG_ERR){
        perror("unable to catch signal");
    }
    for (i=0; i<1000000; i++) {
        sleep(0.01);
    }
}

