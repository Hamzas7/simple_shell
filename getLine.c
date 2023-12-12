#include "shell.h"

/**
 * input_buf - buffers linked commands
 * @info: struct parameter
 * @len: the address of len var
 * @buf: the address of buffer
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t z = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		z = getline(buf, &len_p, stdin);
#else
		z = _getline(info, buf, &len_p);
#endif
		if (z > 0)
		{
			if ((*buf)[z - 1] == '\n')
			{
				(*buf)[z - 1] = '\0';
				z--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			{
				*len = z;
				info->cmd_buf = buf;
			}
		}
	}
	return (z);
}

/**
 * get_input - gets a line minus the newline
 * @info: struct parameter
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf;
	static size_t x, y, len;
	ssize_t w = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	w = input_buf(info, &buf, &len);
	if (w == -1)
		return (-1);
	if (len)
	{
		y = x; 
		p = buf + x;

		check_chain(info, buf, &y, x, len);
		while (y < len)
		{
			if (is_chain(info, buf, &y))
				break;
			y++;
		}

		x = y + 1;
		if (x >= len)
		{
			x = len = 0;
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p;
		return (_strlen(p));
	}

	*buf_p = buf;
	return (w);
}

/**
 * read_buf - reads a buffer
 * @i: size
 * @buf: buffer
 * @info: struct parameter
 *
 * Return: w
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t x = 0;

	if (*i)
		return (0);
	x = read(info->readfd, buf, READ_BUF_SIZE);
	if (x >= 0)
		*i = x;
	return (x);
}

/**
 * _getline - obtains the subsequent line of data from STDIN
 * @info: struct parameter
 * @ptr: address of the buffer's pointer, either NULL or preallocated
 * @length: if not NULL, the size of the preallocated ptr buffer
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t x, len;
	size_t y;
	ssize_t z = 0, w = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		w = *length;
	if (x == len)
		x = len = 0;

	z = read_buf(info, buf, &len);
	if (z == -1 || (z == 0 && len == 0))
		return (-1);

	c = _strchr(buf + x, '\n');
	y = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, w, w ? w + y : y + 1);
	if (!new_p)
		return (p ? free(p), -1 : -1);

	if (w)
		_strncat(new_p, buf + x, y - x);
	else
		_strncpy(new_p, buf + x, y - x + 1);

	w += y - x;
	x = y;
	p = new_p;

	if (length)
		*length = w;
	*ptr = p;
	return (w);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
