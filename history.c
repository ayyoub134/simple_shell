#include "custom_shell.h"

/**
 * get_history_file - gets the history file
 * @shell_info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(shell_info_t *shell_info)
{
    char *directory, *file_path;

    directory = _getenv(shell_info, "HOME=");
    if (!directory)
        return NULL;

    file_path = malloc(sizeof(char) * (_strlen(directory) + _strlen(HISTORY_FILE) + 2));
    if (!file_path)
        return NULL;

    file_path[0] = '\0';
    _strcpy(file_path, directory);
    _strcat(file_path, "/");
    _strcat(file_path, HISTORY_FILE);
    return file_path;
}

/**
 * write_history_to_file - creates a file, or appends to an existing file
 * @shell_info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history_to_file(shell_info_t *shell_info)
{
    ssize_t file_descriptor;
    char *file_name = get_history_file(shell_info);
    list_t *node = NULL;

    if (!file_name)
        return -1;

    file_descriptor = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(file_name);

    if (file_descriptor == -1)
        return -1;

    for (node = shell_info->command_history; node; node = node->next)
    {
        _putsfd(node->str, file_descriptor);
        _putfd('\n', file_descriptor);
    }

    _putfd(BUF_FLUSH, file_descriptor);
    close(file_descriptor);
    return 1;
}

/**
 * read_history_from_file - reads history from file
 * @shell_info: the parameter struct
 *
 * Return: history count on success, 0 otherwise
 */
int read_history_from_file(shell_info_t *shell_info)
{
    int i, last = 0, line_count = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stat;
    char *buffer = NULL, *file_name = get_history_file(shell_info);

    if (!file_name)
        return 0;

    file_descriptor = open(file_name, O_RDONLY);
    free(file_name);

    if (file_descriptor == -1)
        return 0;

    if (!fstat(file_descriptor, &file_stat))
        file_size = file_stat.st_size;

    if (file_size < 2)
        return 0;

    buffer = malloc(sizeof(char) * (file_size + 1));

    if (!buffer)
        return 0;

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = '\0';

    if (read_length <= 0)
        return free(buffer), 0;

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            build_history_list(shell_info, buffer + last, line_count++);
            last = i + 1;
        }
    }

    if (last != i)
        build_history_list(shell_info, buffer + last, line_count++);

    free(buffer);
    shell_info->history_count = line_count;

    while (shell_info->history_count-- >= HISTORY_MAX)
        delete_node_at_index(&(shell_info->command_history), 0);

    renumber_history(shell_info);
    return shell_info->history_count;
}

/**
 * build_history_list - adds entry to a history linked list
 * @shell_info: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @line_count: the history line count, history_count
 *
 * Return: Always 0
 */
int build_history_list(shell_info_t *shell_info, char *buffer, int line_count)
{
    list_t *node = NULL;

    if (shell_info->command_history)
        node = shell_info->command_history;

    add_node_end(&node, buffer, line_count);

    if (!shell_info->command_history)
        shell_info->command_history = node;

    return 0;
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @shell_info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new history count
 */
int renumber_history(shell_info_t *shell_info)
{
    list_t *node = shell_info->command_history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return (shell_info->history_count = i);
}

