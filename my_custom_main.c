#include "my_custom_shell.h"

/**
 * my_custom_main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int my_custom_main(int argc, char **argv)
{
	my_custom_info_t my_custom_info[] = { MY_CUSTOM_INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
	"add $3, %0"
	: "=r" (fd)
	: "r" (fd));

	if (argc == 2)
	{
	fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
		if (errno == EACCES)
		exit(126);
		if (errno == ENOENT)
		{
			_my_custom_eputs(argv[0]);
			_my_custom_eputs(": 0: Can't open ");
			_my_custom_eputs(argv[1]);
			_my_custom_eputchar('\n');
			_my_custom_eputchar(MY_BUF_FLUSH);
			exit(127);
		}
		return (EXIT_FAILURE);
	}
	my_custom_info->readfd = fd;
	}
	my_custom_populate_env_list(my_custom_info);
	my_custom_read_history(my_custom_info);
	my_custom_shell(my_custom_info, argv);
	return (EXIT_SUCCESS);
}

