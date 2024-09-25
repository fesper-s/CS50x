#include "minishell.h"

void	return_dir(t_env **env, int j)
{
	char	**bufferpp;
	char	*buffer;
	char	*pwd;

	bufferpp = split((*env)->env[j], '/');
	buffer = strtrim((*env)->env[j], \
		bufferpp[cmds_count(bufferpp) - 1]);
	free_charpp(bufferpp);
	pwd = strtrim(buffer, "/");
	free(buffer);
	free((*env)->env[j]);
	(*env)->env[j] = strdup(pwd);
	free(pwd);
}

void	relative_path(char *cmd, t_env **env, int j)
{
	char	*buffer;
	char	*pwd;

	if (!strncmp(cmd, "..", 3))
		return_dir(env, j);
	else if (strncmp(cmd, ".", 2))
	{
		buffer = strdup((*env)->env[j]);
		pwd = strjoin(buffer, "/");
		free(buffer);
		buffer = strdup(pwd);
		free(pwd);
		pwd = strjoin(buffer, cmd);
		free(buffer);
		free((*env)->env[j]);
		(*env)->env[j] = strdup(pwd);
		free(pwd);
	}
}

void	chpwd(char *cmd, t_env **env, int j)
{
	char	*buffer;

	if (!strncmp((*env)->env[j], "PWD=", 4))
	{
		if (strncmp(cmd, "/", 2) && cmd[strlen(cmd) - 1] == '/')
			cmd[strlen(cmd) - 1] = 0;
		if (cmd[0] == '/')
		{
			buffer = strjoin("PWD=", cmd);
			free((*env)->env[j]);
			(*env)->env[j] = strdup(buffer);
			free(buffer);
		}
		else
			relative_path(cmd, env, j);
	}
}

char	*tilde_home(char *cmd, char *home)
{
	char	*buffer;
	char	*buffer_aux;

	buffer_aux = strdup(home);
	buffer = strjoin(buffer_aux, &cmd[1]);
	free(buffer_aux);
	free(cmd);
	cmd = strdup(buffer);
	free(buffer);
	return (cmd);
}

char	*no_argincd(char **env)
{
	char	*home;
	int		i;

	home = NULL;
	i = -1;
	while (env[++i])
	{
		if (!strncmp(env[i], "HOME=", 5))
		{
			home = strdup(env[i] + 5);
			break ;
		}
	}
	if (!home)
	{
		print_error("minishell: cd: HOME not set\n");
		g_status = 1;
	}
	else
		chdir(home);
	return (home);
}
