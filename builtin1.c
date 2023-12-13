#include "custom_shell.h"

/**
 * display_history - displays the history list, one command by line, preceded
 *                   with line numbers, starting at 0.
 * @shell_info: Structure containing potential arguments. Used to maintain
 *              constant function prototype.
 *  Return: Always 0
 */
int display_history(shell_info_t *shell_info)
{
    print_list(shell_info->history);
    return (0);
}

/**
 * remove_alias - removes alias by string
 * @shell_info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int remove_alias(shell_info_t *shell_info, char *str)
{
    char *p, c;
    int ret;

    p = _strchr(str, '=');
    if (!p)
        return (1);
    c = *p;
    *p = 0;
    ret = delete_node_at_index(&(shell_info->alias),
                               get_node_index(shell_info->alias, node_starts_with(shell_info->alias, str, -1)));
    *p = c;
    return (ret);
}

/**
 * add_alias - adds alias by string
 * @shell_info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int add_alias(shell_info_t *shell_info, char *str)
{
    char *p;

    p = _strchr(str, '=');
    if (!p)
        return (1);
    if (!*++p)
        return (remove_alias(shell_info, str));

    remove_alias(shell_info, str);
    return (add_node_end(&(shell_info->alias), str, 0) == NULL);
}

/**
 * print_alias_entry - prints an alias entry
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias_entry(list_t *node)
{
    char *p = NULL, *a = NULL;

    if (node)
    {
        p = _strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            _putchar(*a);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * manage_alias - mimics the alias builtin (man alias)
 * @shell_info: Structure containing potential arguments. Used to maintain
 *              constant function prototype.
 *  Return: Always 0
 */
int manage_alias(shell_info_t *shell_info)
{
    int i = 0;
    char *p = NULL;
    list_t *node = NULL;

    if (shell_info->argc == 1)
    {
        node = shell_info->alias;
        while (node)
        {
            print_alias_entry(node);
            node = node->next;
        }
        return (0);
    }
    for (i = 1; shell_info->argv[i]; i++)
    {
        p = _strchr(shell_info->argv[i], '=');
        if (p)
            add_alias(shell_info, shell_info->argv[i]);
        else
            print_alias_entry(node_starts_with(shell_info->alias, shell_info->argv[i], '='));
    }

    return (0);
}

