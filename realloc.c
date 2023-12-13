#include "custom_shell.h"

/**
 * _custom_memset - fills memory with a constant byte
 * @str: the pointer to the memory area
 * @ch: the byte to fill *str with
 * @n: the amount of bytes to be filled
 * Return: (str) a pointer to the memory area str
 */
char *_custom_memset(char *str, char ch, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		str[i] = ch;
	return (str);
}

/**
 * custom_ffree - frees a string of strings
 * @str_arr: string of strings
 */
void custom_ffree(char **str_arr)
{
	char **temp = str_arr;

	if (!str_arr)
		return;
	while (*str_arr)
		free(*str_arr++);
	free(temp);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of the previous block
 * @new_size: byte size of the new block
 * Return: pointer to the old block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (p);
}

