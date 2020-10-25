#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        perror("");
        return 1;
    }

    FILE *fp = fopen(argv[1], "w+");
    long int start = strtol(argv[2], NULL, 10);
    long int end = strtol(argv[3], NULL, 10);
    for (int i = start; i <= end; i++) {
        fprintf(fp, "%d\n", i);
    }

    return 0;
}