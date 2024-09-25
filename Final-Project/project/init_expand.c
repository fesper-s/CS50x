#include "minishell.h"

void	apply_expand(t_line **line, t_env *env, int single_quote, int *j)
{
	int	i;

	i = -1;
	if (!(*line)->cmds[*j])
		return ;
	while ((*line)->cmds[*j][++i])
	{
		if (!single_quote && (*line)->cmds[*j][i] == '$')
		{
			if ((*line)->cmds[*j][i + 1] == '?')
				question_mark(line, *j, -1, -1);
			else if ((*line)->cmds[*j][i + 1])
			{
				expanding(line, env, i, *j);
				if ((*line)->cmds[*j] == NULL)
					break ;
				i = -1;
			}
		}
	}
}

void	expand_var(t_line **line, t_env *env, int j)
{
	int		single_quote;
	void	*head;

	head = *line;
	while (*line)
	{
		single_quote = 0;
		j = -1;
		while ((*line)->cmds[++j])
		{
			if ((*line)->cmds[j][0] == '\'')
				single_quote = 1;
			smart_trim(line, j);
			if (!(*line)->cmds[0])
				break ;
			apply_expand(line, env, single_quote, &j);
		}
		*line = (*line)->next;
	}
	*line = head;
}
