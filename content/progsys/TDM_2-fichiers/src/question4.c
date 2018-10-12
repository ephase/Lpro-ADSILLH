#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 32

int main () {
   char read_error[] = "Error when reading input.\n";
#include <string.h>
    char buff[BUFFER];
    FILE *out, *in;
    in = fdopen(STDIN_FILENO, "r");
    out = fdopen(STDOUT_FILENO,"w");
    int count;
    while ((count = fread(&buff, BUFFER,1,in)) > 0)
    {
        fwrite(&buff, BUFFER,1,out);
    }
    if ( count == -1 ){
        fwrite(&read_error, strlen(read_error) + 1,1,fdopen(STDERR_FILENO,"w"));
    }
    exit(EXIT_SUCCESS);
}
