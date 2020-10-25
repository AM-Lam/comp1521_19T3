#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/errno.h>

int main(int argc, char *argv[]) {
    
    if (argc <= 2) {
        fprintf(stderr, "Usage: <filename> ");
        return 1;
    }

    FILE *fp = fopen(argv[1], "w+");
    if(fp == NULL) { 
        perror("");
        return 1;
    }
    char ch;

    for (int i = 2; i < argc ; i++) {
        ch = strtol(argv[i], NULL, 10); 
        fputc(ch, fp);
    }
    
    fclose(fp);

    return 0;
}