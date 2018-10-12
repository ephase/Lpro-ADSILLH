#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    if (argc > 2) {
        printf("Error : %s must have one argument\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argv[1] == NULL) {
        printf("Execute child process\n");
        exit(EXIT_SUCCESS);
    }
    else {
        int x = atoi(argv[1]);
        printf("Number of children to create : %d\n", x);
        for (int i=0; i < x ; i++){
            int pid = fork();
            if (pid == -1) {
                printf("Error : %s", strerror(errno));
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                // we are in our children
                if ((execl(argv[0],"",NULL)) < 0 ) {
                    printf("Error : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }

        }
    }
    exit(EXIT_SUCCESS);
}
