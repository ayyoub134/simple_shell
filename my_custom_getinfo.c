#include "my_custom_shell.h"
/**
* my_custom_clear_info - initializes info_t struct
* @info: struct address
*/
void my_custom_clear_info(info_t *info)
{
info->arg = NULL;
info->argv = NULL;
info->path = NULL;
info->argc = 0;
}
/**
* my_custom_set_info - initializes info_t struct
* @info: struct address
* @av: argument vector
*/
void my_custom_set_info(info_t *info, char **av)
{
int i = 0;
info->fname = av[0];
if (info->arg)
{
info->argv = my_custom_strtow(info->arg, " \t");
if (!info->argv)
{
info->argv = malloc(sizeof(char *) * 2);
if (info->argv)
{
info->argv[0] = my_custom_strdup(info->arg);
info->argv[1] = NULL;
}
}
for (i = 0; info->argv && info->argv[i]; i++)
;
info->argc = i;
my_custom_replace_alias(info);
my_custom_replace_vars(info);
}
}
/**
* my_custom_free_info - frees info_t struct fields
* @info: struct address
* @all: true if freeing all fields
*/
void my_custom_free_info(info_t *info, int all)
{
my_custom_ffree(info->argv);
info->argv = NULL;
info->path = NULL;
if (all)
{
if (!info->cmd_buf)
free(info->arg);
if (info->env)
my_custom_free_list(&(info->env));
if (info->history)
my_custom_free_list(&(info->history));
if (info->alias)
my_custom_free_list(&(info->alias));
my_custom_ffree(info->environ);
info->environ = NULL;
my_custom_bfree((void **)info->cmd_buf);
if (info->readfd > 2)
close(info->readfd);
my_custom_putchar(BUF_FLUSH);
}
}
