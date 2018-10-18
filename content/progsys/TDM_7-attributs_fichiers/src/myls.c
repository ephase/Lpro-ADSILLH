#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main (int argc, char **argv){
    int i;
    struct stat statbuf; 
    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {
            perror("Unable to get stats");
            continue;
        }
        printf("%s", S_ISDIR(statbuf.st_mode) ? "d" : (S_ISLNK(statbuf.st_mode) ? "l" : "_"));
        printf("%s", (statbuf.st_mode & S_IRUSR) ? "r" : "-");
        printf("%s", (statbuf.st_mode & S_IWUSR) ? "w" : "-");
        printf("%s", (statbuf.st_mode & S_IXUSR) ? "x" : "-");
        
        printf("%s", (statbuf.st_mode & S_IRGRP) ? "r" : "-");
        printf("%s", (statbuf.st_mode & S_IWGRP) ? "w" : "-");
        printf("%s", (statbuf.st_mode & S_IXGRP) ? "x" : "-");

        printf("%s", (statbuf.st_mode & S_IROTH) ? "r" : "-");
        printf("%s", (statbuf.st_mode & S_IWOTH) ? "w" : "-");
        printf("%s", (statbuf.st_mode & S_IXOTH) ? "x" : "-");
        
        printf("\t");

        printf("%ld\t", statbuf.st_nlink);

        printf("%d ", statbuf.st_uid);
        printf("%d ", statbuf.st_gid);

        printf("\t");
        printf("%ld", statbuf.st_size);
        
        printf("\t");
        printf("%s", argv[i]);
        printf("\n");
    }
}
