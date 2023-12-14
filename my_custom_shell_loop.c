#include "my_custom_shell.h"

/**
 * my_custom_shell_loop - main shell loop
 * @my_custom_info: the parameter & return info struct
 * @argv: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int my_custom_shell_loop(my_custom_info_t *my_custom_info, char **argv)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
	my_custom_clear_info(my_custom_info)
	if (my_custom_interactive(my_custom_info))
	_my_custom_puts("$ ");
	_my_custom_eputchar(MY_BUF_FLUSH);
	r = my_custom_get_input(my_custom_info);
	if (r != -1)
	{
		my_custom_set_info(my_custom_info, argv);
		builtin_ret = my_custom_find_builtin(my_custom_info);
		if (builtin_ret == -1)
		my_custom_find_cmd(my_custom_info);
	}
	else if (my_custom_interactive(my_custom_info))
	_my_custom_putchar('\n');
	my_custom_free_info(my_custom_info, 0);
	}
	my_custom_write_history(my_custom_info);
	my_custom_free_info(my_custom_info, 1);
	if (!my_custom_interactive(my_custom_info) && my_custom_info->status)
	exit(my_custom_info->status);
	if (builtin_ret == -2)
	{
	if (my_custom_info->err_num == -1)
		exit(my_custom_info->status);
		exit(my_custom_info->err_num);
	}
	return (builtin_ret);
}

/**
 * my_custom_find_builtin - finds a builtin command
 * @my_custom_info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *	0 if builtin executed successfully,
 *	1 if builtin found but not successful,
 *	-2 if builtin signals exit()
 */
int my_custom_find_builtin(my_custom_info_t *my_custom_info)
{
	int i, built_in_ret = -1;
	my_custom_builtin_table builtintbl[] = {
	{"exit", _my_custom_exit},
	{"env", _my_custom_env},
	{"help", _my_custom_help},
	{"history", _my_custom_history},
	{"setenv", _my_custom_setenv},
	{"unsetenv", _my_custom_unsetenv},
	{"cd", _my_custom_cd},
	{"alias", _my_custom_alias},
	{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
	if (_my_custom_strcmp(my_custom_info->argv[0], builtintbl[i].type) == 0)
	{
	my_custom_info->line_count++;
	built_in_ret = builtintbl[i].func(my_custom_info);
	break;
	}
	return (built_in_ret);
}

/**
 * my_custom_find_cmd - finds a command in PATH
 * @my_custom_info: the parameter & return info struct
 *
 * Return: void
 */
void my_custom_find_cmd(my_custom_info_t *my_custom_info)
{
	char *path = NULL;
	int i, k;

	my_custom_info->path = my_custom_info->argv[0];
	if (my_custom_info->linecount_flag == 1)
	{
	my_custom_info->line_count++;
	my_custom_info->linecount_flag = 0;
	}
	for (i = 0, k = 0; my_custom_info->arg[i]; i++)
	if (!my_custom_is_delim(my_custom_info->arg[i], " \t\n"))
		k++;
	if (!k)
	return;

path = my_custom_find_path(my_custom_info, _my_custom_getenv(my_custom_info, "PATH="), my_custom_info->argv[0]);
	if (path)
	{
	my_custom_info->path = path;
	my_custom_fork_cmd(my_custom_info);
	}
	else
	{
if ((my_custom_interactive(my_custom_info) || _my_custom_getenv(my_custom_info, "PATH=")
|| my_custom_info->argv[0][0] == '/') && my_custom_is_cmd(my_custom_info, my_custom_info->argv[0]))
	my_custom_fork_cmd(my_custom_info);
	else if (*(my_custom_info->arg) != '\n')
	{
	my_custom_info->status = 127;
	my_custom_print_error(my_custom_info, "not found\n");
	}
	}
}

/**
 * my_custom_fork_cmd - forks an exec thread to run cmd
 * @my_custom_info: the parameter & return info struct
 *
 * Return: void
 */
void my_custom_fork_cmd(my_custom_info_t *my_custom_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
	/* TODO: PUT ERROR FUNCTION */
	perror("Error:");
	return;
	}
	if (child_pid == 0)
	{
if (execve(my_custom_info->path, my_custom_info->argv, my_custom_get_environ(my_custom_info)) == -1)
	{
	my_custom_free_info(my_custom_info, 1);
	if (errno == EACCES)
	exit(126);
	exit(1);
	}
	/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
	wait(&(my_custom_info->status));
	if (WIFEXITED(my_custom_info->status))
	{
	my_custom_info->status = WEXITSTATUS(my_custom_info->status);
	if (my_custom_info->status == 126)
	my_custom_print_error(my_custom_info, "Permission denied\n");
	}
	}
}
