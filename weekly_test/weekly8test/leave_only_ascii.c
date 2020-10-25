#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r");
    FILE *temp = fopen("temp", "w+");

    int ch = fgetc(f);
    int i = 0;

    while (ch != EOF) {
        if (isascii(ch)) {
            fputc(ch, temp); 
        } 
        i++;
        ch = fgetc(f);
    }

    fclose(f);
    fclose(temp);

    f = fopen(argv[1], "w+");
    temp = fopen("temp", "r");

    while ((ch = fgetc(temp)) != EOF) {
        fputc(ch, f);
    }
    fclose(f);
    fclose(temp);
    


    return 0;
}