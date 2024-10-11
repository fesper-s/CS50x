#include "sbash.h"

char	**ft_strdupp(char **str)
{
	char	**buffer;
	int		i;

	i = -1;
	buffer = malloc((cmds_count(str) + 1) * sizeof(char *));
	while (str[++i])
		buffer[i] = strdup(str[i]);
	buffer[i] = 0;
	return (buffer);
}

int	free_charpp(char **str)
{
	int	i;

	i = -1;
	if (!str)
		return (0);
	while (str[++i])
		free(str[i]);
	free(str);
	return (0);
}

void	exiting(t_line **line, t_env **env)
{
	free((*line)->cmd);
	free_charpp((*line)->cmds);
	if (*line)
		free(*line);
	free_charpp((*env)->env);
	free(*env);
	printf("exit\n");
}

void	free_two(char **p1, char **p2)
{
	free(p1[0]);
	free(p2[0]);
}

void	free_ppp(char **p, char **pp)
{
	free(p[0]);
	free_charpp(pp);
}

void	free_lstcontent(t_line **buffer)
{
	if ((*buffer)->insert_char)
		free_charpp((*buffer)->insert_char);
	if ((*buffer)->path)
		free((*buffer)->path);
	if ((*buffer)->insert_op)
		free((*buffer)->insert_op);
	if ((*buffer)->cmd)
		free((*buffer)->cmd);
	if ((*buffer)->infile)
		free((*buffer)->infile);
	if ((*buffer)->outfile)
		free((*buffer)->outfile);
}

void	lst_free(t_line **lst)
{
	t_line	*buffer;
	t_line	*del;

	buffer = *lst;
	while (buffer)
	{
		free_charpp(buffer->cmds);
		free_lstcontent(&buffer);
		buffer = buffer->next;
	}
	buffer = *lst;
	while (buffer)
	{
		del = buffer;
		buffer = buffer->next;
		free(del);
	}
	*lst = NULL;
}
