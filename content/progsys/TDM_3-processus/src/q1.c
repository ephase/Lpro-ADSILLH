#include <unistd.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    fork();
    fork();
    fork();
    sleep(10);
    exit(EXIT_SUCCESS);
}
