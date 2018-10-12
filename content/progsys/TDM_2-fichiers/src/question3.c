#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#define  HOLE 10000

int main (int argc, char **argv) {
    int fd = open("td2-q3.txt", O_WRONLY | O_CREAT, S_IRWXU);
    if ( fd < 0) {
        write(STDOUT_FILENO, "Error : can't open file\n", 24);
        exit(EXIT_FAILURE);
    }
    for (int i=0;i<4;i++){
        if ( write(fd, "A", 1) == -1 || lseek(fd, HOLE, SEEK_CUR) == -1) {
            write(STDOUT_FILENO, "Error : can't write to file.\n", 29);
            exit(EXIT_FAILURE);
        }
    }
    if ( close(fd) == -1) {
        write(fd,"Error : can't close file.\n", 26);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
