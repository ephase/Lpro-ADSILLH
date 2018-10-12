#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFFMAX 256
#define OUTPATH "packages.list"

int main (int argc, char **argv) {
    pid_t pid;
    int fds[2], status;
   if (pipe(fds) == -1) {
            perror ("Unable to create pipe");
    }
    
    pid = fork();
    if (pid == -1) {
        perror("Unable to fork");
    }

    else if(pid > 0){
        // father
        printf("Fork : Init father...\n");
        if (close(fds[1]) == -1) {
            perror("Unable to close pipe from parent");
        }
        // Waiting for children to terminate
        char buff[BUFFMAX];
        FILE *fout = fopen(OUTPATH,"w");
        if ( fout == NULL ){
           perror("");
        }
        int n;
        while ((n=read(fds[0], &buff, BUFFMAX)) > 0){
            if ( fwrite(&buff, n, 1, fout) == -1 ){
                perror("Error when writing output");
            }
        }
        if ( fclose(fout) == -1){
            perror("Error ou outp[ut file close");
        }
        wait(&status);
    }
    else {
        // children
        if (close(fds[0]) == -1) {
            perror("Unable to close pipe from child");
        }
        if ( dup2(fds[1], STDOUT_FILENO) != STDOUT_FILENO){
           perror("Unable to duplicate files descriptors");
        }
        if ( close(fds[1]) == -1 ){
            perror ("error on pipe in close");
        }
        execlp("pacman","pacman","-Qs", NULL);
     }
     exit(EXIT_SUCCESS);
}
