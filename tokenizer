#include <stdio.h>
#include <string.h>

/**
 * main - tokenize the input in the command
 *
 * Return: 0
 */

int main()
{
	char command[] = "My Name is Hamza";

	char *token;
	char *delim = " ";

	token = strtok(command, delim);

	while (token != NULL)
	{
		printf("Token: %s\n", token);
		token = strtok(NULL, delim);
	}

	return 0;
}
