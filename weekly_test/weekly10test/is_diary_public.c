#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat s;
    int num = 1;
    char *value = getenv("HOME");
    char path[1000];
    snprintf(path, 1000, "%s/.diary", value);
    if (stat(path, &s) != -1) {
        if ((s.st_mode & S_IROTH) == 0) num = 0;
    } else {
        num = 0;
    }

    printf("%d\n", num);
    return 0;
}