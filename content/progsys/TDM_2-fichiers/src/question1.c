#include <unistd.h>
#include <stdlib.h>

int main () {
    char buffer[32];
    int ret, read_ret;
    int next_read = 1;
    do {
        read_ret = read(STDIN_FILENO, buffer, 1);
        switch ( read_ret ) {
            case -1 :
                write (STDERR_FILENO,"Error when reading\n", 19);
                exit(EXIT_FAILURE);
            case 0:
               next_read = 0;
               break;
            default:
                ret = write(STDOUT_FILENO, buffer, 1);
                if ( ret == -1){
                    write (STDERR_FILENO,"Error when writing\n", 19);
                    exit(EXIT_FAILURE);
                }
        }
    } while (  next_read );
    exit(EXIT_SUCCESS);
}
