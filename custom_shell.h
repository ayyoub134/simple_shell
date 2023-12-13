#ifndef _SHELL_H_
#define _SHELL_H_

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

/* Buffer sizes */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Command chaining constants */
typedef enum {
    CMD_NORM = 0,
    CMD_OR = 1,
    CMD_AND = 2,
    CMD_CHAIN = 3
} CMD_TYPE;

/* Conversion options for convert_number() */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* History settings */
#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

extern char **environ;

/**
 * Struct for singly linked list
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

/**
 * Struct for passing information to functions
 */
typedef struct passinfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    list_t *env;
    list_t *history;
    list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* Pointer to cmd ; chain buffer, for memory management */
    CMD_TYPE cmd_buf_type; /* Command type: CMD_TYPE ||, &&, ; */
    int readfd;
    int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * Struct for builtin commands
 */
typedef struct builtin
{
    char *type;
    int (*func)(info_t *);
} builtin_table;

/* Function prototypes */
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* ... Other function prototypes ... */

#endif /* _SHELL_H_ */

