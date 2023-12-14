#include "my_custom_shell.h"
/**
* my_custom_history - displays the history list, one command by line, preceded
* with line numbers, starting at 0.
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_history(info_t *info)
{
my_custom_print_list(info->history);
return (0);
}
/**
* my_custom_unset_alias - unsets alias to string
* @info: parameter struct
* @str: the string alias
*
* Return: Always 0 on success, 1 on error
*/
int my_custom_unset_alias(info_t *info, char *str)
{
char *p, c;
int ret;
p = my_custom_strchr(str, '=');
if (!p)
return (1);
c = *p;
*p = 0;
ret = my_custom_delete_node_at_index(&(info->alias),
my_custom_get_node_index(info->alias, my_custom_node_starts_with(info->alias, str, -1)));
*p = c;
return (ret);
}
/**
* my_custom_set_alias - sets alias to string
* @info: parameter struct
* @str: the string alias
*
* Return: Always 0 on success, 1 on error
*/
int my_custom_set_alias(info_t *info, char *str)
{
char *p;
p = my_custom_strchr(str, '=');
if (!p)
return (1);
if (!*++p)
return (my_custom_unset_alias(info, str));
my_custom_unset_alias(info, str);
return (my_custom_add_node_end(&(info->alias), str, 0) == NULL);
}
/**
* my_custom_print_alias - prints an alias string
* @node: the alias node
*
* Return: Always 0 on success, 1 on error
*/
int my_custom_print_alias(list_t *node)
{
char *p = NULL, *a = NULL;
if (node)
{
p = my_custom_strchr(node->str, '=');
for (a = node->str; a <= p; a++)
my_custom_putchar(*a);
my_custom_putchar('\'');
my_custom_puts(p + 1);
my_custom_puts("'\n");
return (0);
}
return (1);
}
/**
* my_custom_alias - mimics the alias builtin (man alias)
* @info: Structure containing potential arguments. Used to maintain
* constant function prototype.
* Return: Always 0
*/
int my_custom_alias(info_t *info)
{
int i = 0;
char *p = NULL;
list_t *node = NULL;
if (info->argc == 1)
{
node = info->alias;
while (node)
{
my_custom_print_alias(node);
node = node->next;
}
return (0);
}
for (i = 1; info->argv[i]; i++)
{
p = my_custom_strchr(info->argv[i], '=');
if (p)
my_custom_set_alias(info, info->argv[i]);
else
my_custom_print_alias(my_custom_node_starts_with(info->alias, info->argv[i], '='));
}
return (0);
}
