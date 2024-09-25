#include "minishell.h"

char	*check_for_path(char **env, char *env_path)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!strncmp(env[i], "PATH=", 5))
		{
			env_path = strdup(env[i] + 5);
			return (env_path);
		}
	}
	return (NULL);
}

char	*check_cmdpath(char *env_path, char **path, char *cmd)
{
	char	*buffer;
	char	*cmd_path;
	int		i;

	i = -1;
	while (env_path && path[++i])
	{
		buffer = strjoin(path[i], "/");
		cmd_path = strjoin(buffer, cmd);
		free(buffer);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_charpp(path);
			return (cmd_path);
		}
		free(cmd_path);
	}
	return (NULL);
}

void	validate_dir(char **cmds)
{
	DIR	*dir;
	int	i;
	int	fd;

	if (!strncmp(cmds[0], "expr", 5))
	{
		if (g_status == 0)
			g_status = 1;
		else
			g_status = 0;
		return ;
	}
	g_status = 0;
	i = 0;
	while (cmds[++i])
	{
		dir = opendir(cmds[i]);
		fd = open(cmds[i], O_RDONLY);
		if (!dir && cmds[i] && cmds[i][0] != '-' && fd == -1)
			g_status = 1;
		close(fd);
		if (dir)
			closedir(dir);
	}
}

char	*find_path(t_line **line, t_env **env)
{
	char	*cmd_path;
	char	*env_path;
	char	**path;

	if (!(*line)->cmds[0] || is_builtin(line))
	{
		if (!(*line)->cmds[0])
			g_status = 127;
		return (NULL);
	}
	validate_dir((*line)->cmds);
	env_path = check_for_path((*env)->env, NULL);
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
		return (checking_exe(line, &env_path));
	path = split(env_path, ':');
	cmd_path = check_cmdpath(env_path, path, (*line)->cmds[0]);
	free(env_path);
	if (cmd_path)
		return (cmd_path);
	path_error(path, (*line)->cmds[0]);
	free_charpp(path);
	g_status = 127;
	return (0);
}

int	cmds_count(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

char	*strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (0);
	len_s1 = strlen(s1);
	len_s2 = strlen(s2);
	new_str = malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (new_str == 0)
		return (0);
	strlcpy(new_str, s1, len_s1 + 1);
	strlcat(new_str, s2, len_s1 + len_s2 + 1);
	return (new_str);
}

static int	count(int i, long nbr)
{
	if (nbr == 0)
		i++;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		i++;
	}
	return (i);
}

static char	*putstr(int i, long nbr, char *str)
{
	str[i] = 0;
	if (nbr == 0)
		str[--i] = nbr + 48;
	while (nbr > 0)
	{
		str[--i] = (nbr % 10) + 48;
		nbr = nbr / 10;
	}
	return (&*str);
}

char	*itoa(int n)
{
	int		i;
	char	*str;
	long	nbr;

	i = 0;
	i = count(i, n);
	nbr = (long) n;
	if (nbr < 0)
	{
		nbr = nbr * -1;
		str = malloc((++i + 1) * sizeof(char));
		if (str == 0)
			return (0);
		str[0] = '-';
	}
	else
	{
		str = malloc((i + 1) * sizeof(char));
		if (str == 0)
			return (0);
	}
	str = putstr(i, nbr, str);
	return (str);
}

char	*strtrim(char const *s1, char const *set)
{
	size_t	size_cut;
	char	*result;

	if (!s1 || !set)
		return (0);
	while (*s1 && strchr(set, *s1))
		s1++;
	size_cut = strlen(s1);
	while (*s1 && strchr(set, s1[size_cut]))
		size_cut--;
	result = substr(s1, 0, size_cut + 1);
	return (result);
}

static size_t	wordcount(char const *s, char c)
{
	size_t	i;
	size_t	wordnbr;

	i = 0;
	wordnbr = 0;
	while (s[i])
	{
		if (!(s[i] == c))
		{
			wordnbr++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (wordnbr);
}

static size_t	strsize(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != 0 && s[i] != c)
	{
		if (s[i] == '"')
		{
			i++;
			while (s[i] != '"')
				i++;
		}
		if (s[i] == 39)
		{
			i++;
			while (s[i] != 39)
				i++;
		}
		i++;
	}
	return (i);
}

char	**split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**str;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	str = malloc((wordcount(s, c) + 1) * sizeof(char *));
	if (str == 0)
		return (0);
	while (s[j] != 0 && i < wordcount(s, c))
	{
		while (s[j] == c)
			j++;
		str[i] = substr(&s[j], 0, strsize(&s[j], c));
		j += strsize(&s[j], c);
		i++;
	}
	str[i] = 0;
	return (str);
}

void	putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, strlen(s));
}

char	*substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;

	if (!s)
		return (0);
	if (start > strlen(s))
		return (strdup(""));
	if (strlen(s) - start >= len)
		substr = malloc((len + 1) * sizeof(char));
	else
		substr = malloc((strlen(s) - start + 1) * sizeof(char));
	if (!substr)
		return (0);
	strlcpy(substr, &s[start], len + 1);
	return (substr);
}
