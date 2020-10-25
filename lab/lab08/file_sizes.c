#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 

unsigned long stat_file(char *pathname);

int main(int argc, char *argv[]) {
    
    unsigned long sum = 0;

    for (int arg = 1; arg < argc; arg++) {
        sum += stat_file(argv[arg]);
    }

    printf("Total: %ld bytes\n", (unsigned long)sum);

    return 0;
}

unsigned long stat_file(char *pathname) {
    struct stat s;

    printf("%s: ", pathname);

    if (stat(pathname, &s) != 0) {
        perror(pathname);
        exit(1);
    }
    // File size (bytes)
    printf("%ld bytes\n", (unsigned long)s.st_size);
    return s.st_size;
}