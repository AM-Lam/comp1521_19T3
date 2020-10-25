#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serve_web_page.h"

// pathname is the pathname component of a URL
// if a corresponding file exists the client is sent
// a 200 header assuming file contains HTML
// followed by the file contents
// if a the file can't be opened
// a 404 header is sent with no body
void serve_web_page(char *pathname, FILE *client_stream) {


    FILE *f = fopen(pathname, "r");
    if (f == NULL) {
        fprintf(client_stream, HEADER_404);
        return;
    } else {
        fprintf(client_stream, HEADER_200);
        char *ch = malloc(sizeof(char) * 10000);
        while ((ch = fgets(ch, 10000, f)) != NULL) {
            fputs(ch, client_stream);
        }
        
    }

    fclose(f);
}
