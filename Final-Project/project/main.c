#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argv;
	if (argc != 1)
		return (print_error("This program do not accept arguments\n"));
	minishell(envp);
	return (0);
}
