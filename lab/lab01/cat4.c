// COMP1521 19T3 ... lab 1
// cat4: Copy input to output

#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1000

static void copy(FILE *, FILE *);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        copy (stdin, stdout);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("Can't read %s", argv[i]);
            } else {
                copy (file, stdout);
            }
            fclose(file);
        }
    }
    return EXIT_SUCCESS;
}


// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output) {

    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, input) != NULL)
        fputs(buf, output);

}
