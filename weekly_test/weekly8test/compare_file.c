#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");

    int ch1 = fgetc(f1);
    int ch2 = fgetc(f2);

    int i = 0;

    while (ch1 != EOF || ch2 != EOF) {
        if (ch1 == EOF) {
            printf("EOF on %s\n", argv[1]);
            return 0;
        }

        if (ch2 == EOF) {
            printf("EOF on %s\n", argv[2]);
            return 0;
        }
        
        if (ch1 != ch2) {
            printf("Files differ at byte %d\n", i);
            return 0;
        }

        i++;
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
    }

    printf("Files are identical\n");
    fclose(f1);
    fclose(f2);

    return 0;
}