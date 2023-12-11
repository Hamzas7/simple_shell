#include "shell.h"

/**
 * _printf - a function works as printf
 * @user_input: The input string to be written
 */

void _printf(const char *user_input)
{
	if (write(STDOUT_FILENO, user_input, strlen(user_input)) == -1)
	{
		perror("write");
	}
}
