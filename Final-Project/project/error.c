#include "sbash.h"

int	print_error(char *str)
{
	putstr_fd(str, 2);
	return (1);
}

void	cmd_error(char *cmd)
{
	print_error("sbash: ");
	print_error(cmd);
	print_error(": command not found\n");
}

void	error_display(char *cmd)
{
	cmd_error(cmd);
	g_status = 127;
}

void	dir_error(char *path)
{
	putstr_fd("sbash: cd: no such file or directory: ", 2);
	putstr_fd(path, 2);
	putstr_fd("\n", 2);
	g_status = 1;
}

int	export_error(char *str, char *msg)
{
	putstr_fd("sbash: ", 2);
	putstr_fd(msg, 2);
	putstr_fd(": `", 2);
	putstr_fd(str, 2);
	putstr_fd("': not a valid identifier\n", 2);
	g_status = 1;
	return (1);
}
