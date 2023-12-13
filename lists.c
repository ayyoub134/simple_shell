#include "custom_shell.h"

/**
 * add_node_at_beginning - adds a node to the beginning of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: pointer to the new head
 */
list_t *add_node_at_beginning(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return NULL;

	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return NULL;

	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return NULL;
		}
	}

	new_head->next = *head;
	*head = new_head;
	return new_head;
}

/**
 * add_node_at_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: pointer to the new node
 */
list_t *add_node_at_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *current_node;

	if (!head)
		return NULL;

	current_node = *head;
	new_node = malloc(sizeof(list_t));

	if (!new_node)
		return NULL;

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return NULL;
		}
	}

	if (current_node)
	{
		while (current_node->next)
			current_node = current_node->next;
		current_node->next = new_node;
	}
	else
		*head = new_node;

	return new_node;
}

/**
 * print_list_strings - prints only the str element of a list_t linked list
 * @head: pointer to the first node
 *
 * Return: size of the list
 */
size_t print_list_strings(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}

	return count;
}

/**
 * delete_node_at_index - deletes node at the given index
 * @head: address of pointer to the first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *current_node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return 0;

	if (!index)
	{
		current_node = *head;
		*head = (*head)->next;
		free(current_node->str);
		free(current_node);
		return 1;
	}

	current_node = *head;
	while (current_node)
	{
		if (i == index)
		{
			prev_node->next = current_node->next;
			free(current_node->str);
			free(current_node);
			return 1;
		}

		i++;
		prev_node = current_node;
		current_node = current_node->next;
	}

	return 0;
}

/**
 * free_list_nodes - frees all nodes of a list
 * @head: address of pointer to head node
 *
 * Return: void
 */
void free_list_nodes(list_t **head)
{
	list_t *current_node, *next_node;

	if (!head || !*head)
		return;

	current_node = *head;
	while (current_node)
	{
		next_node = current_node->next;
		free(current_node->str);
		free(current_node);
		current_node = next_node;
	}

	*head = NULL;
}

