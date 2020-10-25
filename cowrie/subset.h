#define SUBSET_H

// subset 0
void do_pwd(char **);
void do_cd(char **);

// subset 1
void do_executable(char **, char *,char **, const posix_spawn_file_actions_t *);

// subset 2
void write_history(char **, char **,char **);
void read_history(char **, int num);
char *find_line(int);
int get_line_message(char *, char **);

// subset 4
void do_redirect(int, int , char **, char *, char **);

// helper function
int argc(char **);
void status(int);
char *find_history(void);
int number_of_line(void);
_Bool isnumber(char *);
void invalid_redir(int);
char **slice_string(char **, int);