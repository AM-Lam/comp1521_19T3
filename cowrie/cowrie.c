// cowrie.c a simple shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <spawn.h>
#include <glob.h>
#include <ctype.h>


// PUT EXTRA `#include'S HERE
#include "subset.h"

#define MAX_LINE_CHARS 1024
#define INTERACTIVE_PROMPT "cowrie> "
#define DEFAULT_PATH "/bin:/usr/bin"
#define WORD_SEPARATORS " \t\r\n"
#define DEFAULT_HISTORY_SHOWN 10

// These characters are always returned as single words
#define SPECIAL_CHARS "!><|"


static void execute_command(char **words, char **path, char **environment);
static void do_exit(char **words);
static int is_executable(char *pathname);
static char **tokenize(char *s, char *separators, char *special_chars);
static void free_tokens(char **tokens);

int main(void) {
    //ensure stdout is line-buffered during autotesting
    setlinebuf(stdout);

    // Environment variables are pointed to by `environ', an array of
    // strings terminated by a NULL value -- something like:
    //     { "VAR1=value", "VAR2=value", NULL }
    extern char **environ;

    // grab the `PATH' environment variable;
    // if it isn't set, use the default path defined above
    char *pathp;
    if ((pathp = getenv("PATH")) == NULL) {
        pathp = DEFAULT_PATH;
    }
    char **path = tokenize(pathp, ":", "");

    char *prompt = NULL;
    // if stdout is a terminal, print a prompt before reading a line of input
    if (isatty(1)) {
        prompt = INTERACTIVE_PROMPT;
    }

    // main loop: print prompt, read line, execute command
    while (1) {
        if (prompt) {
            fputs(prompt, stdout);
        }

        char line[MAX_LINE_CHARS];
        if (fgets(line, MAX_LINE_CHARS, stdin) == NULL) {
            break;
        }

        char **command_words = tokenize(line, WORD_SEPARATORS, SPECIAL_CHARS);
        execute_command(command_words, path, environ);
        free_tokens(command_words);
    }

    free_tokens(path);
    return 0;
}


//
// Execute a command, and wait until it finishes.
//
//  * `words': a NULL-terminated array of words from the input command line
//  * `path': a NULL-terminated array of directories to search in;
//  * `environment': a NULL-terminated array of environment variables.
//
static void execute_command(char **words, char **path, char **environment) {
    assert(words != NULL);
    assert(path != NULL);
    assert(environment != NULL);

    char *program = words[0];

    if (program == NULL) return;

    if (strcmp(program, "exit") == 0) {
        do_exit(words);
        // do_exit will only return if there is an error
        return;
    }

    // subset 2: print out the content in history
    if (strcmp(program, "history") == 0) {
        // if no specific number(words[1]), 
        // get the last 10 lines 
        int num = get_line_message("history", words);
        // if no error is produced
        if (num != -2) read_history(words, num);
    }

    // subset 2: record the command except for '!'
    if (strrchr(words[0], '!') == NULL) 
        write_history(words, path, environment);
        // avoid error message
    if (strcmp(program, "history") == 0) return;

    // subset 3: globbing
    char **result = calloc(MAX_LINE_CHARS, sizeof(char));
    int len = 0;
    // holds pattern expansion
    glob_t matches; 
    for (int i = 0; words[i] != 0; i++) {
        // if glob works, append the filename to the string.
        // if there are no need to glob, just skip it
        if (glob(words[i], GLOB_NOCHECK|GLOB_TILDE, NULL, &matches) == 0) {
            for (int j = 0; j < matches.gl_pathc; j++) {
                result[len] = matches.gl_pathv[j];
                len++;
            }
        } else {
            result[len] = words[i];
            len++;
        }
    }

    if (strcmp(program, "pwd") == 0) {
        do_pwd(words);
        return;
    }

    if (strcmp(program, "cd") == 0) {
        do_cd(result);
        return;
    }

    // subset 2: get the particular message, print and execute it 
    if (strrchr(program, '!') != NULL) {
        // if no specific number(words[1]), 
        // get the latest command
        int num = get_line_message("!", words);
        if (num == -2) return;
        char **command = tokenize(find_line(num), WORD_SEPARATORS, SPECIAL_CHARS);
        execute_command(command, path, environment);
        return;
    }

    // subset 1/4: find the executable path name
    char buf[MAX_LINE_CHARS];
    if (strrchr(program, '/') == NULL) {
        for (int i = 0; path[i] != 0; i++) {
            // append the command(words[0]) to the path
            snprintf(buf, MAX_LINE_CHARS, "%s/%s", path[i], words[0]);
            if (is_executable(buf)) {
                break;
            } 
        }
    } else {
        if (is_executable(program)) {
            snprintf(buf, MAX_LINE_CHARS, "%s", program);
        }
    }

    // subset 4: I/O redirection
    //  * `put': a number to seperate ">" and "<" command with others;
    int position;
    int put = -1;
    for (position = 0; words[position] != 0; position++) {
        if (strrchr(words[position], '<') != NULL) put = 1;
        if (strrchr(words[position], '>') != NULL) put = 0;
        if (put != -1) {
            do_redirect(1, position, words, buf, environment);
            return;
        }
    }

    // subset 1: find the executable path and execute 
    if (is_executable(buf)) {
        do_executable(result, buf, environment, NULL);
        return;
    }

    fprintf(stderr, "%s: command not found\n", program);
    
}

//
// Implement the `exit' shell built-in, which exits the shell.
//
// Synopsis: exit [exit-status]
// Examples:
//     % exit
//     % exit 1
//
static void do_exit(char **words) {
    int exit_status = 0;

    if (words[1] != NULL) {
        if (words[2] != NULL) {
            fprintf(stderr, "exit: too many arguments\n");
        } else {
            char *endptr;
            exit_status = (int)strtol(words[1], &endptr, 10);
            if (*endptr != '\0') {
                fprintf(stderr, "exit: %s: numeric argument required\n",
                        words[1]);
            }
        }
    }

    exit(exit_status);
}


//
// Check whether this process can execute a file.
// Use this function when searching through the directories
// in the path for an executable file
//
static int is_executable(char *pathname) {
    struct stat s;
    return
        // does the file exist?
        stat(pathname, &s) == 0 &&
        // is the file a regular file?
        S_ISREG(s.st_mode) &&
        // can we execute it?
        faccessat(AT_FDCWD, pathname, X_OK, AT_EACCESS) == 0;
}


//
// Split a string 's' into pieces by any one of a set of separators.
//
// Returns an array of strings, with the last element being `NULL';
// The array itself, and the strings, are allocated with `malloc(3)';
// the provided `free_token' function can deallocate this.
//
static char **tokenize(char *s, char *separators, char *special_chars) {
    size_t n_tokens = 0;
    // malloc array guaranteed to be big enough
    char **tokens = malloc((strlen(s) + 1) * sizeof *tokens);


    while (*s != '\0') {
        // We are pointing at zero or more of any of the separators.
        // Skip leading instances of the separators.
        s += strspn(s, separators);

        // Now, `s' points at one or more characters we want to keep.
        // The number of non-separator characters is the token length.
        //
        // Trailing separators after the last token mean that, at this
        // point, we are looking at the end of the string, so:
        if (*s == '\0') {
            break;
        }

        size_t token_length = strcspn(s, separators);
        size_t token_length_without_special_chars = strcspn(s, special_chars);
        if (token_length_without_special_chars == 0) {
            token_length_without_special_chars = 1;
        }
        if (token_length_without_special_chars < token_length) {
            token_length = token_length_without_special_chars;
        }
        char *token = strndup(s, token_length);
        assert(token != NULL);
        s += token_length;

        // Add this token.
        tokens[n_tokens] = token;
        n_tokens++;
    }

    tokens[n_tokens] = NULL;
    // shrink array to correct size
    tokens = realloc(tokens, (n_tokens + 1) * sizeof *tokens);

    return tokens;
}


//
// Free an array of strings as returned by `tokenize'.
//
static void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
