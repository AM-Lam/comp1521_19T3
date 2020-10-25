#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/errno.h>

#define LEN 4

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: <filename> ");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL) { 
        perror("");
        return 1;
    }

    int ch;
    int counter = 0;
    int flag = 0;
    char *buffer = malloc (sizeof(char) * (LEN + 1));
    while ((ch = fgetc(fp)) != EOF) {
        if (counter == LEN) {
            for (int i = 0; i < LEN; i++) printf("%c", buffer[i]);
        }

        if (isprint(ch)) {
            if (flag == 1) {
                printf("%c", ch);
            } else {
                buffer[counter] = ch;
            }
            counter++;
        } else {
            if (flag == 1) {
                printf("\n");
            }
            counter = 0;
            flag = 0;
            buffer[0] = '\0';
        }

        if (counter >= LEN) {
            flag = 1;
        }
        
        
    }

    if (flag == 1) {
        printf("\n");
    }
    fclose(fp);

// it finds 4 bytes in a row containing printable characters, it should print those bytes, 
// and any following bytes containing ASCII printable characters.
}