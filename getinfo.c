#include "custom_shell.h"

/**
 * initialize_shell_info - initializes shell_info_t struct
 * @shell_info: struct address
 */
void initialize_shell_info(shell_info_t *shell_info)
{
    shell_info->argument = NULL;
    shell_info->arguments = NULL;
    shell_info->path = NULL;
    shell_info->argument_count = 0;
}

/**
 * populate_shell_info - initializes shell_info_t struct
 * @shell_info: struct address
 * @argument_vector: argument vector
 */
void populate_shell_info(shell_info_t *shell_info, char **argument_vector)
{
    int i = 0;

    shell_info->filename = argument_vector[0];
    if (shell_info->argument)
    {
        shell_info->arguments = strtow(shell_info->argument, " \t");
        if (!shell_info->arguments)
        {

            shell_info->arguments = malloc(sizeof(char *) * 2);
            if (shell_info->arguments)
            {
                shell_info->arguments[0] = _strdup(shell_info->argument);
                shell_info->arguments[1] = NULL;
            }
        }
        for (i = 0; shell_info->arguments && shell_info->arguments[i]; i++)
            ;
        shell_info->argument_count = i;

        replace_alias(shell_info);
        replace_vars(shell_info);
    }
}

/**
 * deallocate_shell_info - frees shell_info_t struct fields
 * @shell_info: struct address
 * @free_all: true if freeing all fields
 */
void deallocate_shell_info(shell_info_t *shell_info, int free_all)
{
    ffree(shell_info->arguments);
    shell_info->arguments = NULL;
    shell_info->path = NULL;
    if (free_all)
    {
        if (!shell_info->command_buffer)
            free(shell_info->argument);
        if (shell_info->environment)
            free_list(&(shell_info->environment));
        if (shell_info->history)
            free_list(&(shell_info->history));
        if (shell_info->alias)
            free_list(&(shell_info->alias));
        ffree(shell_info->environment_variables);
        shell_info->environment_variables = NULL;
        bfree((void **)shell_info->command_buffer);
        if (shell_info->read_file_descriptor > 2)
            close(shell_info->read_file_descriptor);
        _putchar(BUF_FLUSH);
    }
}

