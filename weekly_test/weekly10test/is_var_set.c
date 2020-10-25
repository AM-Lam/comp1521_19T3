#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int num = 0;
    if (getenv(argv[1]) != NULL) {
        num = 1;
        if (strlen(getenv(argv[1])) == 0) num = 0;
    }

    printf("%d\n", num);
    return 0;
}