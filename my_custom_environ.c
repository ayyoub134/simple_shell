#include "my_custom_shell.h"
/**
* my_custom_env - prints the current environment
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_env(info_t *info)
{
my_custom_print_list_str(info->env);
return (0);
}
/**
* my_custom_getenv - gets the value of an environ variable
* @info: Structure containing potential arguments. Used to maintain
* @name: env var name
*
* Return: the value
*/
char *my_custom_getenv(info_t *info, const char *name)
{
list_t *node = info->env;
char *p;
while (node)
{
p = my_custom_starts_with(node->str, name);
if (p && *p)
return (p);
node = node->next;
}
return (NULL);
}
/**
* my_custom_setenv - Initialize a new environment variable,
* or modify an existing one
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_setenv(info_t *info)
{
if (info->argc != 3)
{
my_custom_eputs("Incorrect number of arguements\n");
return (1);
}
if (my_custom_setenv_func(info, info->argv[1], info->argv[2]))
return (0);
return (1);
}
/**
* my_custom_unsetenv - Remove an environment variable
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_unsetenv(info_t *info)
{
int i;
if (info->argc == 1)
{
my_custom_eputs("Too few arguements.\n");
return (1);
}
for (i = 1; i <= info->argc; i++)
my_custom_unsetenv_func(info, info->argv[i]);
return (0);
}
/**
* my_custom_populate_env_list - populates env linked list
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_populate_env_list(info_t *info)
{
list_t *node = NULL;
size_t i;
for (i = 0; environ[i]; i++)
my_custom_add_node_end(&node, environ[i], 0);
info->env = node;
return (0);
}
