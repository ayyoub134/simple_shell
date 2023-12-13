#include "custom_shell.h"

/**
 * string_to_int_with_error - converts a string to an integer with error handling
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int string_to_int_with_error(char *s)
{
    int i = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++; /* Skip the optional '+' sign */

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX)
                return (-1); /* Overflow error */
        }
        else
            return (-1); /* Non-numeric character found */
    }
    return (result);
}

/**
 * print_error_message - prints an error message
 * @info: the parameter & return info struct
 * @error_type: string containing specified error type
 * Return: None
 */
void print_error_message(info_t *info, char *error_type)
{
    _eputs(info->filename);
    _eputs(": ");
    print_line_number(info->line_count, STDERR_FILENO);
    _eputs(": ");
    _eputs(info->argv[0]);
    _eputs(": ");
    _eputs(error_type);
}

/**
 * print_line_number - prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int print_line_number(int input, int fd)
{
    int (*__putchar)(char) = _putchar;
    int i, count = 0;
    unsigned int _abs_, current;

    if (fd == STDERR_FILENO)
        __putchar = _eputchar;
    if (input < 0)
    {
        _abs_ = -input;
        __putchar('-');
        count++;
    }
    else
        _abs_ = input;
    current = _abs_;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (_abs_ / i)
        {
            __putchar('0' + current / i);
            count++;
        }
        current %= i;
    }
    __putchar('0' + current);
    count++;

    return (count);
}

/**
 * convert_to_string - converts a number to a string
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convert_to_string(long int num, int base, int flags)
{
    static char *array;
    static char buffer[50];
    char sign = 0;
    char *ptr;
    unsigned long n = num;

    if (!(flags & CONVERT_UNSIGNED) && num < 0)
    {
        n = -num;
        sign = '-';
    }

    array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;

    return (ptr);
}

/**
 * remove_comments_from_line - replaces first instance of '#' with '\0'
 * @line: address of the string to modify
 *
 * Return: None
 */
void remove_comments_from_line(char *line)
{
    int i;

    for (i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '#' && (!i || line[i - 1] == ' '))
        {
            line[i] = '\0';
            break;
        }
    }
}

