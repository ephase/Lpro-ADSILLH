#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main () {
    int fd,i;
    char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

    //open file
    fd = open("tp2-q2.txt",O_WRONLY | O_CREAT, S_IRWXU); 
    if ( fd == -1 ) {
        write(STDERR_FILENO,"Error when opening file\n", 25);
        exit(EXIT_FAILURE);
    }
    for (i=0;i<26;i++) {
        if ( write(fd, &alphabet[i], 1) == -1 || write(fd,"\n",1) == -1){
            write (STDERR_FILENO,"Error when writing\n", 19);
            exit(EXIT_FAILURE);
        }
    }
    //Clode file
    if ( close(fd) == -1){
        write(STDERR_FILENO, "Can't close file.\n", 18);
    }
    exit(EXIT_SUCCESS);
}
