#include "custom_shell.h"

/**
 * check_interactive_mode - returns true if shell is in interactive mode
 * @shell_info: struct containing shell information
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int check_interactive_mode(shell_info_t *shell_info)
{
	return (isatty(STDIN_FILENO) && shell_info->input_fd <= 2);
}

/**
 * is_character_delimiter - checks if character is a delimiter
 * @character: the character to check
 * @delimiter: the delimiter string
 * Return: 1 if true, 0 if false
 */
int is_character_delimiter(char character, char *delimiter)
{
	while (*delimiter)
		if (*delimiter++ == character)
			return (1);
	return (0);
}

/**
 * is_alpha - checks for alphabetic character
 * @c: The character to input
 * Return: 1 if c is alphabetic, 0 otherwise
 */
int is_alpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * convert_to_integer - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int convert_to_integer(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

