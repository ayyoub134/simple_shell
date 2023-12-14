#include "my_custom_shell.h"
/**
* my_custom_get_history_file - gets the history file
* @info: parameter struct
*
* Return: allocated string containg history file
*/
char *my_custom_get_history_file(info_t *info)
{
char *buf, *dir;
dir = my_custom_getenv(info, "HOME=");
if (!dir)
return (NULL);
buf = my_custom_malloc(sizeof(char) * (my_custom_strlen(dir) + my_custom_strlen(HIST_FILE) + 2));
if (!buf)
return (NULL);
buf[0] = 0;
my_custom_strcpy(buf, dir);
my_custom_strcat(buf, "/");
my_custom_strcat(buf, HIST_FILE);
return (buf);
}
/**
* my_custom_write_history - creates a file, or appends to an existing file
* @info: the parameter struct
*
* Return: 1 on success, else -1
*/
int my_custom_write_history(info_t *info)
{
ssize_t fd;
char *filename = my_custom_get_history_file(info);
list_t *node = NULL;
if (!filename)
return (-1);
fd = my_custom_open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
my_custom_free(filename);
if (fd == -1)
return (-1);
for (node = info->history; node; node = node->next)
{
my_custom_putsfd(node->str, fd);
my_custom_putfd('\n', fd);
}
my_custom_putfd(BUF_FLUSH, fd);
my_custom_close(fd);
return (1);
}
/**
* my_custom_read_history - reads history from file
* @info: the parameter struct
*
* Return: histcount on success, 0 otherwise
*/
int my_custom_read_history(info_t *info)
{
int i, last = 0, linecount = 0;
ssize_t fd, rdlen, fsize = 0;
struct stat st;
char *buf = NULL, *filename = my_custom_get_history_file(info);
if (!filename)
return (0);
fd = my_custom_open(filename, O_RDONLY);
my_custom_free(filename);
if (fd == -1)
return (0);
if (!my_custom_fstat(fd, &st))
fsize = st.st_size;
if (fsize < 2)
return (0);
buf = my_custom_malloc(sizeof(char) * (fsize + 1));
if (!buf)
return (0);
rdlen = my_custom_read(fd, buf, fsize);
buf[fsize] = 0;
if (rdlen <= 0)
return (my_custom_free(buf), 0);
my_custom_close(fd);
for (i = 0; i < fsize; i++)
if (buf[i] == '\n')
{
buf[i] = 0;
my_custom_build_history_list(info, buf + last, linecount++);
last = i + 1;
}
if (last != i)
my_custom_build_history_list(info, buf + last, linecount++);
my_custom_free(buf);
info->histcount = linecount;
while (info->histcount-- >= HIST_MAX)
my_custom_delete_node_at_index(&(info->history), 0);
my_custom_renumber_history(info);
return (info->histcount);
}
/**
* my_custom_build_history_list - adds entry to a history linked list
* @info: Structure containing potential arguments. Used to maintain
* @buf: buffer
* @linecount: the history linecount, histcount
*
* Return: Always 0
*/
int my_custom_build_history_list(info_t *info, char *buf, int linecount)
{
list_t *node = NULL;
if (info->history)
node = info->history;
my_custom_add_node_end(&node, buf, linecount);
if (!info->history)
info->history = node;
return (0);
}
/**
* my_custom_renumber_history - renumbers the history linked list after changes
* @info: Structure containing potential arguments. Used to maintain
*
* Return: the new hist
