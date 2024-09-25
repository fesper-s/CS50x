#ifndef MINISHELL_H
# define MINISHELL_H

	# include <stdio.h>
	# include <unistd.h>
	# include <fcntl.h>
	# include <stdlib.h>
	# include <signal.h>
	# include <readline/readline.h>
	# include <readline/history.h>
	# include <sys/ioctl.h>
	# include <sys/wait.h>
	# include <dirent.h>
	# include <string.h>

	typedef struct s_line
	{
		char			*cmd;
		pid_t			child;
		char			**cmds;
		char			*infile;
		char			*outfile;
		int				infile_id;
		int				outfile_id;
		int				extract_op;
		char			*insert_op;
		int				insert_len;
		int				is_insert;
		char			**insert_char;
		char			*path;
		struct s_line	*next;
	}					t_line;

	typedef struct s_env
	{
		char	**env;
	}			t_env;

	extern int	g_status;

	void	cmd_process(t_line **line, t_env **env);
	int		organize_line(t_line **line);
	void	minishell(char **envp);
	int		check_files(char **cmds);
	int		check_operator(t_line **line, char **cmds, int i);
	void	check_builtins(t_line **line, t_env **env, int size);
	void	check_for_pipes(t_line **line, char **cmds, int i, int j);
	char	*check_space(char *cmd);
	void	rm_insert_op(t_line **line, char **cmds, int i);
	char	*put_space(char *cmd, int x);
	int		check_quote_on(char *cmd);
	int		check_double_pipes(t_line *line);
	int		cut_cmd(t_line **line);
	void	ft_lst_add_back(t_line **lst, t_line *new);
	t_line	*ft_lst_last(t_line *lst);
	t_line	*ft_lst_new(char **cmds);
	int		ft_lst_size(t_line *lst);
	void	ft_lst_add_next(t_line **lst, t_line *new);
	int		init_files(t_line **line);
	int		init_cmds(t_line **line, char **split);
	void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe);
	int		print_error(char *str);
	void	cmd_error(char *cmd);
	void	error_display(char *cmd);
	void	dir_error(char *path);
	int		export_error(char *str, char *msg);
	void	path_error(char **path, char *cmd);
	void	check_dir_error(char *str);
	char	*checking_exe(t_line **line, char **env_path);
	void	signals(void);
	void	handle_sigint(int signum);
	char	*check_for_path(char **env, char *env_path);
	char	*check_cmdpath(char *env_path, char **path, char *cmd);
	char	*find_path(t_line **line, t_env **env);
	int		cmds_count(char **split);
	void	free_lstcontent(t_line **buffer);
	void	smart_trim(t_line **line, int index);
	int		handle_cd(char **cmds, t_env **env);
	int		handle_pwd(t_env *env);
	int		handle_env(t_env *env);
	int		handle_builtins(char **cmds, t_env **env);
	int		tilpipe(char **cmds);
	int		count_export_len(char *str);
	int		check_dir(char **cmds);
	int		is_builtin(t_line **line);
	int		is_flag(char **cmds, int i);
	void	check_newline(char **cmds, int *newline, int *buffer, int i);
	int		handle_echo(char **cmds);
	void	return_dir(t_env **env, int j);
	void	relative_path(char *cmd, t_env **env, int j);
	void	chpwd(char *cmd, t_env **env, int j);
	char	*tilde_home(char *cmd, char *home);
	char	*no_argincd(char **env);
	void	add_to_env(char *cmd, t_env **env);
	int		exporting(char *cmd, t_env **env);
	int		count_cmdlen(char *cmd);
	int		handle_export(char **cmds, t_env **env);
	void	attr_buffer(char ***buffer, char *cmd, char **env);
	int		check_cmd_env(char **env, char *cmd);
	int		handle_unset(char **cmds, t_env **env);
	char	**ft_strdupp(char **str);
	int		free_charpp(char **str);
	void	exiting(t_line **line, t_env **env);
	void	free_two(char **p1, char **p2);
	void	free_ppp(char **p, char **pp);
	void	lst_free(t_line **lst);
	void	apply_expand(t_line **line, t_env *env, int single_quote, int *j);
	void	expand_var(t_line **line, t_env *env, int j);
	void	expanding(t_line **line, t_env *env, int j, int index);
	void	null_chexpand(t_line **line, t_env *l_env, int index, int i);
	char	*init_values(char **aux, char *env, t_line **line, int index);
	void	chexpand(t_line **line, char *env, int index, int i);
	int		search_varenv(char *cmds, t_env *env, int j);
	void	question_mark(t_line **line, int index, int i, int j);
	int		til_dollar_sign(char *str);
	int		check_varenv(char **env, char *str);
	int		exp_malloc(char *buf);
	void	print_insert(t_line **line);
	void	insert_operation(t_line **line, char *eof);
	void	return_null(int signum);
	void	insert_exec(t_line **line);
	void	check_heredocs(t_line **line, char **cmds, int *i);
	void	handle_cmd(t_line **line, t_env **env);
	void	exec_cmds(t_line **line, t_env **env, int *fd, int *fdd);
	void	check_for_builtins(char **cmds);
	void	pipeline(t_line **line, t_env **env, int size);
	void	ch_cmd_data(t_line **line, char ***buffer, int j);
	void	open_files(t_line **line);
	int		file_len(char **cmd);
	char	*strjoin(char const *s1, char const *s2);
	char	*strtrim(char const *s1, char const *set);
	char	*itoa(int n);
	char	**split(char const *s, char c);
	void	putstr_fd(char *s, int fd);
	char	*substr(char const *s, unsigned int start, size_t len);

#endif
