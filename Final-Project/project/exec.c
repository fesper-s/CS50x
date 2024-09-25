#include "minishell.h"

void	handle_cmd(t_line **line, t_env **env)
{
	handle_builtins((*line)->cmds, env);
	print_insert(line);
	if (!(*line)->cmds[0] && !(*line)->insert_op && !(*line)->infile && \
		!(*line)->outfile)
		printf("minishell: : command not found\n");
	exit(EXIT_SUCCESS);
}

void	exec_cmds(t_line **line, t_env **env, int *fd, int *fdd)
{
	if ((*line)->child == 0)
	{
		if ((*line)->infile_id > 0)
			dup2((*line)->infile_id, 0);
		else
			dup2(*fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		if ((*line)->outfile_id > 0)
			dup2((*line)->outfile_id, 1);
		close(fd[0]);
		if (check_dir((*line)->cmds) || !(*line)->path)
			handle_cmd(line, env);
		else
			execve((*line)->path, (*line)->cmds, (*env)->env);
	}
	else
	{
		close(fd[1]);
		*fdd = fd[0];
		(*line) = (*line)->next;
	}
}

void	check_for_builtins(char **cmds)
{
	if (!cmds[0])
		return ;
	if (!strncmp(cmds[0], "echo", 5))
		g_status = 0;
	if (!strncmp(cmds[0], "pwd", 4))
		g_status = 0;
	if (!strncmp(cmds[0], "env", 4))
		g_status = 0;
}

void	pipeline(t_line **line, t_env **env, int size)
{
	int	fd[2];
	int	fdd;

	fdd = 0;
	while ((*line))
	{
		(*line)->path = find_path(line, env);
		insert_exec(line);
		check_for_builtins((*line)->cmds);
		pipe(fd);
		(*line)->child = fork();
		if ((*line)->child == -1 && print_error("Error: fork\n"))
			break ;
		else
			exec_cmds(line, env, fd, &fdd);
	}
	while (size--)
		waitpid(-1, NULL, 0);
}
