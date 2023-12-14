#ifndef _MY_CUSTOM_SHELL_H_
#define _MY_CUSTOM_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define MY_READ_BUF_SIZE 1024
#define MY_WRITE_BUF_SIZE 1024
#define MY_BUF_FLUSH -1

/* for command chaining */
#define MY_CMD_NORM 0
#define MY_CMD_OR 1
#define MY_CMD_AND 2
#define MY_CMD_CHAIN 3

/* for convert_number() */
#define MY_CONVERT_LOWERCASE 1
#define MY_CONVERT_UNSIGNED 2

/* 1 if using system getline() */
#define MY_USE_GETLINE 0
#define MY_USE_STRTOK 0

#define MY_HIST_FILE ".my_custom_shell_history"
#define MY_HIST_MAX 4096

extern char **my_custom_environ;

/**
 * struct my_custom_list - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct my_custom_list
{
	int num;
	char *str;
	struct my_custom_list *next;
} my_custom_list_t;

/**
 *struct my_custom_passinfo - contains pseudo-arguments to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguments
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct my_custom_passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	my_custom_list_t *env;
	my_custom_list_t *history;
	my_custom_list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
}
my_custom_info_t;

#define MY_CUSTOM_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct my_custom_builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
*/
typedef struct my_custom_builtin
{
	char *type;
	int (*func)(my_custom_info_t *);
} my_custom_builtin_table;


/* my_custom_shloop.c */
int my_custom_shell(my_custom_info_t *, char **);
int find_my_custom_builtin(my_custom_info_t *);
void find_my_custom_cmd(my_custom_info_t *);
void fork_my_custom_cmd(my_custom_info_t *);

/* my_custom_parser.c */
int is_my_custom_cmd(my_custom_info_t *, char *);
char *dup_my_custom_chars(char *, int, int);
char *find_my_custom_path(my_custom_info_t *, char *, char *);

/* my_custom_loophsh.c */
int my_custom_loophsh(char **);

/* my_custom_errors.c */
void _my_custom_eputs(char *);
int _my_custom_eputchar(char);
int _my_custom_putfd(char c, int fd);
int _my_custom_putsfd(char *str, int fd);

/* my_custom_string.c */
int _my_custom_strlen(char *);
int _my_custom_strcmp(char *, char *);
char *my_custom_starts_with(const char *, const char *);
char *_my_custom_strcat(char *, char *);

/* my_custom_string1.c */
char *_my_custom_strcpy(char *, char *);
char *_my_custom_strdup(const char *);
void _my_custom_puts(char *);
int _my_custom_putchar(char);

/* my_custom_exits.c */
char *_my_custom_strncpy(char *, char *, int);
char *_my_custom_strncat(char *, char *, int);
char *_my_custom_strchr(char *, char);

/* my_custom_tokenizer.c */
char **my_custom_strtow(char *, char *);
char **my_custom_strtow2(char *, char);

/* my_custom_realloc.c */
char *_my_custom_memset(char *, char, unsigned int);
void my_custom_ffree(char **);
void *_my_custom_realloc(void *, unsigned int, unsigned int);

/* my_custom_memory.c */
int my_custom_bfree(void **);

/* my_custom_atoi.c */
int my_custom_interactive(my_custom_info_t *);
int my_custom_is_delim(char, char *);
int my_custom_is_alpha(int);
int my_custom_atoi(char *);

/* my_custom_errors1.c */
int _my_custom_erratoi(char *);
void my_custom_print_error(my_custom_info_t *, char *);
int my_custom_print_d(int, int);
char *my_custom_convert_number(long int, int, int);
void my_custom_remove_comments(char *);

/* my_custom_builtin.c */
int _my_custom_exit(my_custom_info_t *);
int _my_custom_cd(my_custom_info_t *);
int _my_custom_help(my_custom_info_t *);

/* my_custom_builtin1.c */
int _my_custom_history(my_custom_info_t *);
int _my_custom_alias(my_custom_info_t *);

/* my_custom_getline.c */
ssize_t my_custom_get_input(my_custom_info_t *);
int _my_custom_getline(my_custom_info_t *, char **, size_t *);
void my_custom_sigint_handler(int);

/* my_custom_getinfo.c */
void my_custom_clear_info(my_custom_info_t *);
void my_custom_set_info(my_custom_info_t *, char **);
void my_custom_free_info(my_custom_info_t *, int);

/* my_custom_environ.c */
char *_my_custom_getenv(my_custom_info_t *, const char *);
int _my_custom_env(my_custom_info_t *);
int _my_custom_setenv(my_custom_info_t *);
int _my_custom_unsetenv(my_custom_info_t *);
int my_custom_populate_env_list(my_custom_info_t *);

/* my_custom_getenv.c */
char **my_custom_get_environ(my_custom_info_t *);
int _my_custom_unsetenv(my_custom_info_t *, char *);
int _my_custom_setenv(my_custom_info_t *, char *, char *);

/* my_custom_history.c */
char *my_custom_get_history_file(my_custom_info_t *info);
int my_custom_write_history(my_custom_info_t *info);
int my_custom_read_history(my_custom_info_t *info);
int my_custom_build_history_list(my_custom_info_t *info, char *buf, int linecount);
int my_custom_renumber_history(my_custom_info_t *info);

/* my_custom_lists.c */
my_custom_list_t *my_custom_add_node(my_custom_list_t **, const char *, int);
my_custom_list_t *my_custom_add_node_end(my_custom_list_t **, const char *, int);
size_t my_custom_print_list_str(const my_custom_list_t *);
int my_custom_delete_node_at_index(my_custom_list_t **, unsigned int);
void my_custom_free_list(my_custom_list_t **);

/* my_custom_lists1.c */
size_t my_custom_list_len(const my_custom_list_t *);
char **my_custom_list_to_strings(my_custom_list_t *);
size_t my_custom_print_list(const my_custom_list_t *);
my_custom_list_t *my_custom_node_starts_with(my_custom_list_t *, char *, char);
ssize_t my_custom_get_node_index(my_custom_list_t *, my_custom_list_t *);

/* my_custom_vars.c */
int my_custom_is_chain(my_custom_info_t *, char *, size_t *);
void my_custom_check_chain(my_custom_info_t *, char *, size_t *, size_t, size_t);
int my_custom_replace_alias(my_custom_info_t *);
int my_custom_replace_vars(my_custom_info_t *);
int my_custom_replace_string(char **, char *);

#endif

