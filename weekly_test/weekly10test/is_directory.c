#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat s;
    int num = 1;
    if (stat(argv[1], &s) != -1) {
        if (S_ISDIR(s.st_mode) == 0) num = 0;
    } else {
        num = 0;
    }

    printf("%d\n", num);
    return 0;
}