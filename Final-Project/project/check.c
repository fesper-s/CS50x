#include "minishell.h"

int	check_files(char **cmds)
{
	int	infile;
	int	outfile;
	int	i;

	i = -1;
	infile = 0;
	outfile = 0;
	while (cmds[++i])
	{
		if (cmds[i][0] == '<' && cmds[i][1] != '<')
			infile++;
		if (cmds[i][0] == '>' && cmds[i][1] != '>')
			outfile++;
		if (infile > 1 || outfile > 1)
		{
			print_error("Error: Multiples files\n");
			return (0);
		}
		if (cmds[i][0] == '|')
		{
			infile = 0;
			outfile = 0;
		}
	}
	return (1);
}

int	check_operator(t_line **line, char **cmds, int i)
{
	while (cmds[++i])
	{
		if ((cmds[i][0] == '<' || cmds[i][0] == '>') && !cmds[i + 1])
		{
			print_error("zsh: parse error\n");
			return (0);
		}
		else if ((cmds[i][0] == '<' || cmds[i][0] == '>') && \
			(cmds[i + 1][0] == '<' || cmds[i + 1][0] == '>'))
		{
			print_error("zsh: parse error\n");
			return (0);
		}
		else if (!strncmp(cmds[i], "<<<", 3) || \
			!strncmp(cmds[i], ">>>", 3))
		{
			print_error("Error: multiples '<' or '>' operator\n");
			return (0);
		}
		else
			check_heredocs(line, cmds, &i);
		if (i == -1)
			break ;
	}
	return (1);
}

void	check_builtins(t_line **line, t_env **env, int size)
{
	if ((*line)->cmds[0] && size == 1)
	{
		if (!strncmp((*line)->cmds[0], "export", 7))
			handle_export((*line)->cmds, env);
		if (!strncmp((*line)->cmds[0], "unset", 6))
			handle_unset((*line)->cmds, env);
		if (!strncmp((*line)->cmds[0], "cd", 3))
			handle_cd((*line)->cmds, env);
	}
}

void	check_for_pipes(t_line **line, char **cmd, int i, int j)
{
	char	**b_p;
	char	**a_p;

	b_p = NULL;
	a_p = NULL;
	if (cmds_count(cmd) != tilpipe(cmd))
	{
		b_p = malloc((tilpipe(cmd) + 1) * sizeof(char *));
		while (cmd[++i])
		{
			if (cmd[i][0] == '|')
			{
				a_p = malloc((cmds_count(cmd) - tilpipe(cmd)) * sizeof(char *));
				while (cmd[++i])
					a_p[j++] = strdup(cmd[i]);
				break ;
			}
			b_p[i] = strdup(cmd[i]);
		}
		b_p[tilpipe(cmd)] = 0;
		a_p[j] = 0;
		init_linked_list(line, b_p, a_p);
	}
}

char	*check_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<' && (cmd[i + 1] != '<' && cmd[i + 1] != ' ' && \
			cmd[i + 1]))
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i] != '<' && cmd[i + 1] == '<')
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] == '>' && (cmd[i + 1] != '>' && cmd[i + 1] != ' ' \
			&& cmd[i + 1]))
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i] != '>' && cmd[i + 1] == '>')
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] == '|' && cmd[i + 1] != ' ' && cmd[i + 1])
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i + 1] == '|')
			cmd = put_space(cmd, i + 1);
		else
			i++;
	}
	return (cmd);
}
