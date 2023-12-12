#include "shell.h"

/**
 * get_environ - returns a copy of the string array in our environment.
 * @info: returns a duplicate of our environment's string array. Used to maintain
 *          constant function prototype.
 * Return: 0
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Take out a variable from the environment
 * @var: the environment variable string
 * @info: Organize with potential areas of disagreement. Used to maintain
 *        constant function prototype.
 *  Return: If deleted, 1; else, 0
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t x = 0;
	char *ptr;

	if (!node || !var)
		return (0);

	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), x);
			x = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		x++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Set up a fresh environment variable,
 *             or modify an existing one
 * @value: string env var value
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 *  Return: 0
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *ptr;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
