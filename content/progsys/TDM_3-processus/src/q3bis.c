#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    if (argv[1] == NULL) {
        printf("Error : %s must have one argument\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Execute programme number %s\n",argv[1]); 
    int r = atoi(argv[1]) - 1;
    if (r > 0){
        int pid = fork();
        if (pid == -1) {
            printf("Error : %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // we are in our children
            char args[12] = "";
            sprintf(args,"%i", r);
            if ((execl(argv[0], argv[0], args, NULL)) < 0 ) {
                printf("Error : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        printf("This is the end!\n");
    }
    exit(EXIT_SUCCESS);
}
