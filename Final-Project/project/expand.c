#include "sbash.h"

void	expanding(t_line **line, t_env *env, int j, int index)
{
	int	env_posi;

	env_posi = search_varenv((*line)->cmds[index], env, j);
	if (env_posi == -1)
		null_chexpand(line, env, index, -1);
	else
		chexpand(line, env->env[env_posi], index, -1);
}

void	null_chexpand(t_line **line, t_env *l_env, int index, int i)
{
	char	*buffer;
	char	*aux;
	int		j;

	buffer = strdup((*line)->cmds[index]);
	free((*line)->cmds[index]);
	aux = malloc(sizeof(char) * exp_malloc(buffer));
	j = -1;
	while (buffer[++i])
	{
		if (buffer[i] == '$' && check_varenv(l_env->env, &buffer[i + 1]))
			aux[++j] = buffer[i];
		else if (buffer[i] == '$' && buffer[i + 1])
			i += count_export_len(&buffer[i + 1]);
		else
			aux[++j] = buffer[i];
	}
	aux[++j] = '\0';
	(*line)->cmds[index] = strdup(aux);
	if (j == 0)
	{
		free((*line)->cmds[index]);
		(*line)->cmds[index] = NULL;
	}
	free_two(&buffer, &aux);
}

char	*init_values(char **aux, char *env, t_line **line, int index)
{
	char	*buffer;

	aux[0] = strdup(&env[count_cmdlen(env) + 1]);
	buffer = strdup((*line)->cmds[index]);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (strlen(buffer) - \
		count_cmdlen(env) + strlen(aux[0])));
	return (buffer);
}

void	chexpand(t_line **line, char *env, int index, int i)
{
	char	*buffer;
	char	*aux;
	int		j;
	int		k;
	int		dollar;

	dollar = 0;
	buffer = init_values(&aux, env, line, index);
	k = -1;
	while (buffer[++i])
	{
		if (!dollar && buffer[i] == '$')
		{
			dollar = 1;
			j = -1;
			while (aux[++j])
				(*line)->cmds[index][++k] = aux[j];
			i += count_cmdlen(env);
		}
		else
			(*line)->cmds[index][++k] = buffer[i];
	}
	(*line)->cmds[index][k + 1] = 0;
	free_two(&buffer, &aux);
}
