#include "my_custom_shell.h"
/**
* my_custom_is_chain - test if current char in buffer is a chain delimiter
* @info: the parameter struct
* @buf: the char buffer
* @p: address of current position in buf
*
* Return: 1 if chain delimiter, 0 otherwise
*/
int my_custom_is_chain(info_t *info, char *buf, size_t *p)
{
size_t j = *p;
if (buf[j] == '|' && buf[j + 1] == '|')
{
buf[j] = 0;
j++;
info->cmd_buf_type = CMD_OR;
}
else if (buf[j] == '&' && buf[j + 1] == '&')
{
buf[j] = 0;
j++;
info->cmd_buf_type = CMD_AND;
}
else if (buf[j] == ';') /* found end of this command */
{
buf[j] = 0; /* replace semicolon with null */
info->cmd_buf_type = CMD_CHAIN;
}
else
return (0);
*p = j;
return (1);
}
/**
* my_custom_check_chain - checks if we should continus on last status
* @info: the parameter struct
* @buf: the char buffer
* @p: address of current position in buf
* @i: starting position in buf
* @len: length of buf
*
* Return: Void
*/
void my_custom_check_chain(info_t *info,
		char *buf, size_t *p, size_t i, size_t len)
{
size_t j = *p;
if (info->cmd_buf_type == CMD_AND)
{
if (info->status)
{
buf[i] = 0;
j = len;
}
}
if (info->cmd_buf_type == CMD_OR)
{
if (!info->status)
{
buf[i] = 0;
j = len;
}
}
*p = j;
}
/**
* my_custom_replace_alias - replaces an aliases in the tokenized string
* @info: the parameter struct
*
* Return: 1 if replaced, 0 otherwise
*/
int my_custom_replace_alias(info_t *info)
{
int i;
list_t *node;
char *p;
for (i = 0; i < 10; i++)
{
node = my_custom_node_starts_with(info->alias, info->argv[0], '=');
if (!node)
return (0);
my_custom_free(info->argv[0]);
p = my_custom_strchr(node->str, '=');
if (!p)
return (0);
p = my_custom_strdup(p + 1);
if (!p)
return (0);
info->argv[0] = p;
}
return (1);
}
/**
* my_custom_replace_vars - replaces vars in the tokenized string
* @info: the parameter struct
*
* Return: 1 if replaced, 0 otherwise
*/
int my_custom_replace_vars(info_t *info)
{
int i = 0;
list_t *node;
for (i = 0; info->argv[i]; i++)
{
if (info->argv[i][0] != '$' || !info->argv[i][1])
continue;
if (!my_custom_strcmp(info->argv[i], "$?"))
{
my_custom_replace_string(&(info->argv[i]),
my_custom_strdup(my_custom_convert_number(info->status, 10, 0)));
continue;
}
if (!my_custom_strcmp(info->argv[i], "$$"))
{
my_custom_replace_string(&(info->argv[i]),
my_custom_strdup(my_custom_convert_number(my_custom_getpid(), 10, 0)));
continue;
}
node = my_custom_node_starts_with(info->env, &info->argv[i][1], '=');
if (node)
{
my_custom_replace_string(&(info->argv[i]),
my_custom_strdup(my_custom_strchr(node->str, '=') + 1));
continue;
}
my_custom_replace_string(&info->argv[i], my_custom_strdup(""));
}
return (0);
}
/**
* my_custom_replace_string - replaces string
* @old: address of old string
* @new: new string
*
* Return: 1 if replaced, 0 otherwise
*/
int my_custom_replace_string(char **old, char *new)
{
my_custom_free(*old);
*old = new;
return (1);
}
