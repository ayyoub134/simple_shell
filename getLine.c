#include "custom_shell.h"

/**
 * buffer_chained_commands - buffers chained commands
 * @shell_info: parameter struct
 * @buffer: address of buffer
 * @buffer_length: address of buffer length var
 *
 * Return: bytes read
 */
ssize_t buffer_chained_commands(shell_info_t *shell_info, char **buffer, size_t *buffer_length)
{
    ssize_t read_bytes = 0;
    size_t buffer_position = 0;

    if (!*buffer_length) /* if nothing left in the buffer, fill it */
    {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, sigintHandler);
#if USE_GETLINE
        read_bytes = getline(buffer, buffer_length, stdin);
#else
        read_bytes = _getline(shell_info, buffer, buffer_length);
#endif
        if (read_bytes > 0)
        {
            if ((*buffer)[read_bytes - 1] == '\n')
            {
                (*buffer)[read_bytes - 1] = '\0'; /* remove trailing newline */
                read_bytes--;
            }
            shell_info->line_count_flag = 1;
            remove_comments(*buffer);
            build_history_list(shell_info, *buffer, shell_info->history_count++);
            {
                *buffer_length = read_bytes;
                shell_info->command_buffer = buffer;
            }
        }
    }
    return read_bytes;
}

/**
 * get_input_line - gets a line minus the newline
 * @shell_info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input_line(shell_info_t *shell_info)
{
    static char *buffer; /* the ';' command chain buffer */
    static size_t buffer_position, iterator, buffer_length;
    ssize_t read_bytes = 0;
    char **buffer_pointer = &(shell_info->argument), *pointer;

    _putchar(BUF_FLUSH);
    read_bytes = buffer_chained_commands(shell_info, &buffer, &buffer_length);
    if (read_bytes == -1) /* EOF */
        return -1;
    if (buffer_length) /* we have commands left in the chain buffer */
    {
        iterator = buffer_position; /* init new iterator to current buffer position */
        pointer = buffer + buffer_position; /* get pointer for return */

        check_chain(shell_info, buffer, &iterator, buffer_position, buffer_length);
        while (iterator < buffer_length) /* iterate to semicolon or end */
        {
            if (is_chain(shell_info, buffer, &iterator))
                break;
            iterator++;
        }

        buffer_position = iterator + 1; /* increment past nulled ';'' */
        if (buffer_position >= buffer_length) /* reached end of buffer? */
        {
            buffer_position = buffer_length = 0; /* reset position and length */
            shell_info->command_buffer_type = CMD_NORM;
        }

        *buffer_pointer = pointer; /* pass back pointer to current command position */
        return _strlen(pointer); /* return length of current command */
    }

    *buffer_pointer = buffer; /* else not a chain, pass back buffer from _getline() */
    return read_bytes; /* return length of buffer from _getline() */
}

/**
 * read_buffer - reads a buffer
 * @shell_info: parameter struct
 * @buffer: buffer
 * @position: size
 *
 * Return: read_bytes
 */
ssize_t read_buffer(shell_info_t *shell_info, char *buffer, size_t *position)
{
    ssize_t read_bytes = 0;

    if (*position)
        return 0;
    read_bytes = read(shell_info->read_file_descriptor, buffer, READ_BUFFER_SIZE);
    if (read_bytes >= 0)
        *position = read_bytes;
    return read_bytes;
}

/**
 * _getline - gets the next line of input from STDIN
 * @shell_info: parameter struct
 * @pointer: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated pointer buffer if not NULL
 *
 * Return: buffer_size
 */
int _getline(shell_info_t *shell_info, char **pointer, size_t *length)
{
    static char buffer[READ_BUFFER_SIZE];
    static size_t buffer_position, buffer_length;
    size_t read_size;
    ssize_t read_bytes = 0, buffer_size = 0;
    char *buffer_pointer = NULL, *new_buffer_pointer = NULL, *character;

    buffer_pointer = *pointer;
    if (buffer_pointer && length)
        buffer_size = *length;
    if (buffer_position == buffer_length)
        buffer_position = buffer_length = 0;

    read_bytes = read_buffer(shell_info, buffer, &buffer_length);
    if (read_bytes == -1 || (read_bytes == 0 && buffer_length == 0))
        return -1;

    character = _strchr(buffer + buffer_position, '\n');
    read_size = character ? 1 + (unsigned int)(character - buffer) : buffer_length;
    new_buffer_pointer = _realloc(buffer_pointer, buffer_size, buffer_size ? buffer_size + read_size : read_size + 1);
    if (!new_buffer_pointer) /* MALLOC FAILURE! */
        return (buffer_pointer ? free(buffer_pointer), -1 : -1);

    if (buffer_size)
        _strncat(new_buffer_pointer, buffer + buffer_position, read_size - buffer_position);
    else
        _strncpy(new_buffer_pointer, buffer + buffer_position, read_size - buffer_position + 1);

    buffer_size += read_size - buffer_position;
    buffer_position = read_size;
    buffer_pointer = new_buffer_pointer;

    if (length)
        *length = buffer_size;
    *pointer = buffer_pointer;
    return buffer_size;
}

/**
 * sigintHandler - blocks ctrl-C
 * @signal_number: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int signal_number)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}

