#include "custom_shell.h"

/**
 * print_error_string - prints an error string to stderr
 * @error_string: the error string to be printed
 *
 * Return: Nothing
 */
void print_error_string(char *error_string)
{
    int i = 0;

    if (!error_string)
        return;
    while (error_string[i] != '\0')
    {
        write_error_char(error_string[i]);
        i++;
    }
}

/**
 * write_error_char - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_error_char(char c)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * write_to_file_descriptor - writes the character c to given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_to_file_descriptor(char c, int fd)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * print_to_file_descriptor - prints an input string to a given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters put
 */
int print_to_file_descriptor(char *str, int fd)
{
    int i = 0;

    if (!str)
        return (0);
    while (*str)
    {
        i += write_to_file_descriptor(*str++, fd);
    }
    return (i);
}

