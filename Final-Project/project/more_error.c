#include "minishell.h"

void	path_error(char **path, char *cmd)
{	
	if (path)
		error_display(cmd);
	else
	{
		print_error("minishell: ");
		print_error(cmd);
		print_error(": No such file or directory\n");
		g_status = 127;
	}
}

void	check_dir_error(char *str)
{
	print_error("minishell: ");
	print_error(str);
	print_error(": is a directory\n");
	g_status = 126;
}

char	*checking_exe(t_line **line, char **env_path)
{
	g_status = 126;
	if (!strncmp((*line)->cmds[0], "/bin/", 5) || \
		!strncmp((*line)->cmds[0], "/usr/bin/", 9) || \
		!strncmp((*line)->cmds[0], "./", 2))
		g_status = 0;
	free(env_path[0]);
	return (strdup((*line)->cmds[0]));
}
