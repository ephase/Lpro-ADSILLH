#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void output_error() {
    fprintf(stderr,"Error : %s\n",strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    char *file = "tp2_q5.txt";
    FILE *f = fopen(file,"w");
    char m[56] = "\0";
    char ret = '\n';
    if ( f == NULL ){
        output_error();
    }
    for(char i='a' ;i<='z' ; i++){
        strncat(m,&i,1);
        strncat(m,&ret,1);
    }
    fwrite(&m,strlen(m) + 1,1,f);
    if ( ferror(f) ) {
        output_error();
    } 
    fclose(f);
    if ( ferror(f) ){
        output_error();
    }
    exit(EXIT_SUCCESS);
}
