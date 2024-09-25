#include "minishell.h"

void	add_to_env(char *cmd, t_env **env)
{
	int		i;
	char	**buffer;

	i = cmds_count((*env)->env);
	buffer = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while ((*env)->env[++i])
		buffer[i] = strdup((*env)->env[i]);
	buffer[i] = strdup(cmd);
	buffer[i + 1] = 0;
	free_charpp((*env)->env);
	(*env)->env = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (buffer[++i])
		(*env)->env[i] = strdup(buffer[i]);
	(*env)->env[i] = 0;
	free_charpp(buffer);
}

int	exporting(char *cmd, t_env **env)
{
	int	i;
	int	isattr;

	isattr = 0;
	if (isalpha(cmd[0]) || cmd[0] == '_')
	{
		i = 0;
		while (cmd[++i])
		{
			if (cmd[i] == '=')
			{
				isattr = 1;
				break ;
			}
			if (!isalnum(cmd[i]))
				return (export_error(cmd, "export"));
		}
		if (isattr)
			add_to_env(cmd, env);
		g_status = 0;
	}
	else
		export_error(cmd, "export");
	return (0);
}

int	count_cmdlen(char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] && cmd[len] != '=')
		len++;
	return (len);
}

int	handle_export(char **cmds, t_env **env)
{
	int	changed;
	int	i;
	int	j;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		changed = 0;
		j = -1;
		while ((*env)->env[++j])
		{
			if (!strncmp(cmds[i], (*env)->env[j], count_cmdlen(cmds[i])) \
				&& (*env)->env[j][count_cmdlen(cmds[i])] == '=')
			{
				free((*env)->env[j]);
				(*env)->env[j] = strdup(cmds[i]);
				changed = 1;
				g_status = 0;
			}
		}
		if (!changed)
			exporting(cmds[i], env);
	}
	return (1);
}
