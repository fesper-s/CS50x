#include "minishell.h"

int	is_flag(char **cmds, int i)
{
	int	j;

	while (--i >= 1)
	{
		j = 0;
		if (cmds[i][j] == '-')
			j++;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				return (0);
			j++;
		}
	}
	return (1);
}

void	check_newline(char **cmds, int *newline, int *buffer, int i)
{
	int	j;

	if (cmds[i] && !strncmp(cmds[i], "-n", 2))
	{
		*newline = 1;
		j = 1;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				break ;
			j++;
			if (!cmds[i][j] && is_flag(cmds, i))
			{
				*buffer = 1;
				*newline = 0;
			}
		}
	}
}

int	handle_echo(char **cmds)
{
	int	newline;
	int	buffer;
	int	i;

	i = 0;
	buffer = 0;
	while (cmds[++i])
	{
		newline = 1;
		check_newline(cmds, &newline, &buffer, i);
		if (cmds[i] && newline)
			printf("%s", cmds[i]);
		if (cmds[i + 1] && newline)
			printf(" ");
	}
	if (!buffer)
		printf("\n");
	return (1);
}
