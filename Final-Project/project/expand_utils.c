#include "sbash.h"

int	search_varenv(char *cmds, t_env *env, int j)
{
	char	*buffer;
	int		i;

	buffer = strdup(&cmds[j + 1]);
	i = -1;
	while (env->env[++i])
	{
		if (buffer[til_dollar_sign(buffer)] == '$' && \
			!strncmp(env->env[i], buffer, strlen(buffer) - \
			strlen(&buffer[til_dollar_sign(buffer)])) && \
			env->env[i][til_dollar_sign(buffer)] == '=')
		{
			free(buffer);
			return (i);
		}
		else if (!strncmp(env->env[i], buffer, count_export_len(buffer)) \
			&& env->env[i][count_export_len(buffer)] == '=')
		{
			free(buffer);
			return (i);
		}
	}
	free(buffer);
	return (-1);
}

void	question_mark(t_line **line, int index, int i, int j)
{
	int		k;
	char	*buffer;
	char	*aux;

	aux = itoa(g_status);
	buffer = strdup((*line)->cmds[index]);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (strlen(buffer) - 2 + \
		strlen(aux) + 1));
	while (buffer[++i])
	{
		if (!strncmp(&buffer[i], "$?", 2))
		{
			i++;
			k = -1;
			while (aux[++k])
				(*line)->cmds[index][++j] = aux[k];
		}
		else
			(*line)->cmds[index][++j] = buffer[i];
	}
	(*line)->cmds[index][j + 1] = 0;
	free(aux);
	free(buffer);
}

int	til_dollar_sign(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '$')
		len++;
	return (len);
}

int	check_varenv(char **env, char *str)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!strncmp(env[i], str, count_cmdlen(env[i])) && \
			env[i][count_export_len(str)] == '=')
			return (1);
	}
	return (0);
}

int	exp_malloc(char *buf)
{
	int	len;
	int	rm;

	len = strlen(buf);
	rm = 0;
	if (buf[0] == '$')
		rm = til_dollar_sign(&buf[1]) + 1;
	else
	{
		rm = til_dollar_sign(&buf[til_dollar_sign(buf)] + 1);
		if (rm == 0)
			rm = 1;
	}
	len = len - rm + 1;
	return (len);
}
