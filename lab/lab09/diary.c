#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *value = getenv("HOME");
    char buf[100];
    snprintf(buf, 100, "%s/.diary", value);

    FILE *f = fopen(buf, "a");

    for (int i = 1; i < argc; i++) {
        fprintf(f, "%s ", argv[i]);
    }
    fprintf(f, "\n");

    fclose(f);
    return 0;
}