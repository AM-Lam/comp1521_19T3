#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/errno.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        perror("");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    int ch;
    long int i = 0;

    while ((ch = fgetc(fp)) != EOF) {

        printf("byte %4ld: %3d 0x%02x ", i, ch, ch);

        if (isprint(ch)) {
            printf("'%c'", ch);
        } 

        printf("\n");
        i++;

    }
    
    fclose(fp);

    return 0;
}