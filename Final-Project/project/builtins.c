#include "minishell.h"

void	smart_trim(t_line **line, int index)
{
	char	*temp;

	temp = NULL;
	if ((*line)->cmds[index][0] == '\'' || (*line)->cmds[index][\
		strlen((*line)->cmds[index]) - 1] == '\'')
		temp = strtrim((*line)->cmds[index], "\'");
	else if ((*line)->cmds[index][0] == '\"' || (*line)->cmds[index][\
		strlen((*line)->cmds[index]) - 1] == '\"')
		temp = strtrim((*line)->cmds[index], "\"");
	else
		temp = strdup((*line)->cmds[index]);
	if (temp)
	{
		free((*line)->cmds[index]);
		(*line)->cmds[index] = strdup(temp);
		free(temp);
	}
	if (!strlen((*line)->cmds[0]))
	{
		free_charpp((*line)->cmds);
		(*line)->cmds = malloc(sizeof(char *));
		(*line)->cmds[0] = 0;
	}
}

int	handle_cd(char **cmds, t_env **env)
{
	char	*home;
	int		i;

	if (cmds[1] && cmds[1][0] == '~')
		cmds[1] = tilde_home(cmds[1], getenv("HOME"));
	if (!cmds[1])
		home = no_argincd((*env)->env);
	else if (chdir(cmds[1]) < 0)
	{
		dir_error(cmds[1]);
		g_status = 1;
		return (2);
	}
	i = -1;
	while ((*env)->env[++i])
	{
		if (!cmds[1] && home)
			chpwd(home, env, i);
		else if (cmds[1])
			chpwd(cmds[1], env, i);
	}
	if (!cmds[1] && home)
		free(home);
	g_status = 0;
	return (1);
}

int	handle_pwd(t_env *env)
{
	int		i;
	char	*buffer;
	char	*buffer_aux;

	i = 0;
	while (strncmp(env->env[i], "PWD=", 4))
		i++;
	buffer_aux = strdup(env->env[i]);
	buffer = strtrim(buffer_aux, "PWD=");
	printf("%s\n", buffer);
	free(buffer);
	free(buffer_aux);
	return (1);
}

int	handle_env(t_env *env)
{
	int		i;

	g_status = 0;
	i = -1;
	while (env->env[++i])
		printf("%s\n", env->env[i]);
	return (1);
}

int	handle_builtins(char **cmds, t_env **env)
{
	if (!cmds[0])
		return (0);
	if (!strncmp(cmds[0], "echo", 5))
		return (handle_echo(cmds));
	if (!strncmp(cmds[0], "pwd", 4))
		return (handle_pwd(*env));
	if (!strncmp(cmds[0], "env", 4))
		return (handle_env(*env));
	return (0);
}
