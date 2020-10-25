#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r");
    int ch = fgetc(f);
    int i = 0;

    while (ch != EOF) {
        if (!isascii(ch)) {
            printf("%s: byte %d is non-ASCII\n", argv[1], i);
            return 0;
        }
        i++;
        ch = fgetc(f);
    }

    printf("%s is all ASCII\n", argv[1]);
    fclose(f);

    return 0;
}