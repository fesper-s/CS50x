#include "minishell.h"

void	print_insert(t_line **line)
{
	int	i;

	i = -1;
	if ((*line)->insert_char && (*line)->next && (*line)->next->is_insert)
	{
		while (++i < (*line)->insert_len)
			printf("%s\n", (*line)->insert_char[i]);
	}
}

void	insert_operation(t_line **line, char *eof)
{
	int		i;
	char	**buffer;

	i = -1;
	buffer = NULL;
	(*line)->insert_len++;
	if (!(*line)->insert_char)
	{
		(*line)->insert_char = malloc(2 * sizeof(char *));
		(*line)->insert_char[0] = strdup(eof);
		(*line)->insert_char[1] = 0;
	}
	else
	{
		buffer = malloc((cmds_count((*line)->insert_char) + 2) * \
			sizeof(char *));
		while ((*line)->insert_char[++i])
			buffer[i] = strdup((*line)->insert_char[i]);
		buffer[i] = strdup(eof);
		buffer[i + 1] = 0;
		free_charpp((*line)->insert_char);
		(*line)->insert_char = ft_strdupp(buffer);
		free_charpp(buffer);
	}
}

void	return_null(int signum)
{
	if (signum == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_status = 666;
	}
}

void	insert_exec(t_line **line)
{
	char	*eof;

	if ((*line)->insert_op)
	{
		while (1)
		{
			signal(SIGINT, return_null);
			eof = readline("> ");
			if (!eof || g_status == 666)
			{
				g_status = 1;
				if (!eof)
					printf("\n");
				break ;
			}
			g_status = 0;
			if (!strncmp((*line)->insert_op, eof, strlen(eof) + 1))
				break ;
			insert_operation(line, eof);
			free(eof);
		}
		free(eof);
	}
}

void	check_heredocs(t_line **line, char **cmds, int *i)
{
	if (!strncmp(cmds[*i], ">>", 2) && cmds[*i + 1])
			(*line)->extract_op = 1;
	else if (!strncmp(cmds[*i], "<<", 2) && cmds[*i + 1])
	{
		smart_trim(line, *i + 1);
		(*line)->insert_op = strdup(cmds[*i + 1]);
		*i = -1;
		if (cmds_count(cmds) > 2)
			rm_insert_op(line, cmds, *i);
		else
		{
			free_charpp((*line)->cmds);
			(*line)->cmds = malloc(sizeof(char *));
			(*line)->cmds[0] = 0;
		}
		*i = -1;
	}
}
