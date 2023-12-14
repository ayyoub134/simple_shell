#include "my_custom_shell.h"
/**
* my_custom_eputs - prints an input string to stderr
* @str: the string to be printed
*
* Return: Nothing
*/
void my_custom_eputs(char *str)
{
int i = 0;
if (!str)
return;
while (str[i] != '\0')
{
my_custom_eputchar(str[i]);
i++;
}
}
/**
* my_custom_eputchar - writes the character c to stderr
* @c: The character to print
*
* Return: On success 1.
* On error, -1 is returned, and errno is set appropriately.
*/
int my_custom_eputchar(char c)
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
* my_custom_putfd - writes the character c to given fd
* @c: The character to print
* @fd: The file descriptor to write to
*
* Return: On success 1.
* On error, -1 is returned, and errno is set appropriately.
*/
int my_custom_putfd(char c, int fd)
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
* my_custom_putsfd - prints an input string to a given file descriptor
* @str: the string to be printed
* @fd: the file descriptor to write to
*
* Return: the number of chars put
*/
int my_custom_putsfd(char *str, int fd)
{
int i = 0;
if (!str)
return (0);
while (*str)
{
i += my_custom_putfd(*str++, fd);
}
return (i);
}
