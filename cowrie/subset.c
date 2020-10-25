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

#include "subset.h"

#define TRUE 1
#define FALSE 0

#define MAX_LINE_CHARS 1024

// do the pwd command in shell
// get the pathname of the current working directory
void do_pwd(char **words) {

    if (argc(words) > 1) fprintf(stderr, "pwd: too many arguments\n");

    char pathname[MAX_LINE_CHARS];
    if (getcwd(pathname, sizeof pathname) == NULL) 
        fprintf(stderr, "getcwd: command is not executable\n");
    
    printf("current directory is '%s'\n", pathname);
}

// do the cd command in shell
void do_cd(char **words) {
    int len = argc(words);

    if (argc(words) > 2) {
        fprintf(stderr, "pwd: too many arguments\n");
    // go back or change a directory 
    // error if the directory name is invalid
    } else if (len == 2 && chdir(words[1]) != 0){
        fprintf(stderr, "cd: %s: No such file or directory\n", words[1]);
    // go back to home directory
    } else if (len == 1) {
        if (chdir(getenv("HOME")) != 0) {
            fprintf(stderr, "chdir: command is not executable\n");
        }
    }
}

// handle all the error messages 
// and execute the file that passed in
//  * `file': a NULL-terminated string of the executable file;
void do_executable(char **words, char *file, char **environ, 
                    const posix_spawn_file_actions_t *actions) {
    pid_t pid;
    int exit_status = 0;

    if (posix_spawn(&pid, file, actions, NULL, words, environ) != 0) 
        fprintf(stderr, "spawn: command is not executable\n");

    if (waitpid(pid, &exit_status, 0) == -1) 
        fprintf(stderr, "waitpid: command is not executable\n");

    printf("%s ", file);
    status(WEXITSTATUS(exit_status));
}

// subset 2: write the command to the history.file 
// with a new line at the end
void write_history(char **words, char **path, char **environment) {
    char *file = find_history();
    FILE *f = fopen(file, "a");
    for (int i = 0; words[i] != 0; i++) fprintf(f, "%s ", words[i]);
    fprintf(f, "\n");
    fclose(f);
    return;
}

// subset 2: command 'history'
// print out the latest certain number of lines in history.file
//  * `flag': a counter to keep track of when do we print the index;
//  * `index': a counter to keep track of number of line, start with 0;
void read_history(char **words, int num) {
    char *file = find_history();
    FILE *f = fopen(file, "r");
    if (f == NULL) return;
    int ch;
    int index = 0;
    int flag = 1;

    while ((ch = fgetc(f)) != EOF) {
        // when index get to the last n(=num (variable that we put in)) lines
        // print out the line
        if (index > number_of_line() - num - 1) {
            // only print out the index when there is a new line
            if (flag == 1) {
                fprintf(stdout, "%d: ", index);
                flag = 0;
            }
            fprintf(stdout, "%c", ch);
        }

        if (ch == '\n') {
            index++;
            flag = 1;
        }
    }
    fclose(f);
}

// subset2: command '!'
// get to the certain number of line, 
// store the message in a string and return it 
//  * `flag': a counter which decide when to start storing the string
//            also the index of the string;
//  * `count': a counter to keep track of number of line, start with 0;
char *find_line(int num) {
    char *file = find_history();
    FILE *f = fopen(file, "r");
    int ch;
    int count = 0;
    int flag = 0;
    char *str = calloc(MAX_LINE_CHARS, sizeof(char));

    while ((ch = getc(f)) != EOF) {
        if (count == num) flag++;
        
        if (flag > 0) {
            fprintf(stdout, "%c", ch);
            str[flag - 1] = ch;
        }
        // end the string when finishing the line
        if (ch == '\n') {
            count++;
            if (flag > 0) break;
        } 
    }
    fclose(f);
    return str;
}

// subset 2: handle the error messages in 'history' and '!'
// return the value of the line in those function also
// based of the words[1](given value)
// or the default on 'history' and '!'
// if error, return -2 to escape
int get_line_message(char *command, char **words) {
    if (words[1] == NULL) 
        return (strrchr(command, '!') != NULL) ? number_of_line() - 1: 10;

    if (argc(words) > 2) {
        fprintf(stderr, "%s: too many arguments\n", command);
        return -2;
    }
    if (isnumber(words[1])) {
        fprintf(stderr, "%s: %s: numeric argument required\n", command, words[1]);
        return -2;
    }
    if (number_of_line() - 1 < atoi(words[1])) {
        fprintf(stderr, "%s: invalid history reference\n", command);
        return -2;
    }
    return atoi(words[1]);
}

// subset 4: input or output redirect
void do_redirect(int num, int pos, char **words, char *path, char **environment) {
    if (num != 0 && num != 1) return;
    
    if (argc(words) < 3) 
        invalid_redir(num);

    if (pos == 1) 
        if (num == 0) invalid_redir(num);

    // create a pipe
    int pipe_file_descriptors[2];
    if (pipe(pipe_file_descriptors) == -1) {
        perror("pipe");
        return;
    }

    // create a list of file actions to be carried out on spawned process
    posix_spawn_file_actions_t actions;
    if (posix_spawn_file_actions_init(&actions) != 0) {
        perror("posix_spawn_file_actions_init");
        return;
    }

    // tell spawned process to close unused read end of pipe
    // without this - spawned process would not receive EOF
    // when read end of the pipe is closed below,
    // because spawned process also has the write-end open
    // deadlock will result
    if (posix_spawn_file_actions_addclose(&actions, pipe_file_descriptors[num]) != 0) {
        perror("posix_spawn_file_actions_init");
        return;
    }

    // switch the number around for replace file desciptor
    int flipnum = (num == 1) ? 0 : 1;

    // tell spawned process to replace file descriptor 1 (stdout)
    // with write end of the pipe
    if (posix_spawn_file_actions_adddup2(&actions, pipe_file_descriptors[flipnum], 
                                        flipnum) != 0) {
        perror("posix_spawn_file_actions_adddup2");
        return;
    }

    char **new_words = slice_string(words, pos);
    if (new_words == NULL) new_words = words;
    do_executable(new_words, path, environment, &actions);

    // close unused read end of pipe
    close(pipe_file_descriptors[flipnum]);

    // the mode depends on the I/O
    char *mode = (num == 1) ? "w" : "r";
    FILE *f = fdopen(pipe_file_descriptors[num], mode);
    if (f == NULL) {
        perror("fdopen");
        return;
    }

    if (num == 0) {
        // read a line from read-end of pipe
        char line[MAX_LINE_CHARS];
        if (fgets(line, sizeof(line), f) == NULL) {
            fprintf(stderr, "no output from file\n");
            return;
        }

        // printf("%s\n", line);
    }

    fclose(f);
}

/*  HELPER FUNCTION below  */

// Get the length of a char **string
int argc(char **words) {
    int len = 0;
    for (;words[len] != NULL; len++); 
    return len;
}

// Print out the exit status
void status(int num) {
    printf("exit status = %d\n", num);
} 

// find the address of the $HOME/.cowrie_history and return it
char *find_history(void) {
    char *value = getenv("HOME");
    char *buf = malloc(sizeof(char) * MAX_LINE_CHARS);
    snprintf(buf, MAX_LINE_CHARS, "%s/.cowrie_history", value);
    return buf;
}


// find the total number of line in history.file and return it
int number_of_line(void) {
    char *file = find_history();
    FILE *f = fopen(file, "r");
    if (f == NULL) return 'n';
    int count = 0;
    int ch;
    while ((ch = getc(f)) != EOF) if (ch == '\n') count++;
    fclose(f);
    return count;
}

// To check if the string is number or not
// If the string is number, will return FALSE
_Bool isnumber(char *valid) {
    if (valid == NULL) return FALSE;
    if (valid[0] == '0') return FALSE;
    for (int i = 0; valid[i] != 0; i++) {
        if (isalpha(valid[i])) return TRUE;
    }
    return FALSE;
}

// Print out invalid redirection
void invalid_redir(int num) {
    char *put;
    put = (num == 0) ? "output" : "input";
    fprintf(stderr, "invalid %s redirection\n", put);
} 

// to cut out part of the string 
// from pos onwards
char **slice_string(char **str, int pos) {
    char **newstr = calloc(sizeof(char **), MAX_LINE_CHARS);
    for (int i = 0; str[i] != 0; i++) {
        if (i != pos && i != pos + 1){
            newstr[i] = str[i];
        }
    }
    return newstr;

}