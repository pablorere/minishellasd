#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = copy_envp(envp);
	shell->exit_status = 0;
	shell->tokens = NULL;
	shell->cmds = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmds)
	{
		free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
}

void	process_line(t_shell *shell, char *line)
{
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->cmds)
		free_cmds(shell->cmds);
	shell->tokens = tokenize(line);
	if (!shell->tokens)
		return ;
	expand_tokens(shell->tokens, shell);
	shell->cmds = parse_tokens(shell->tokens);
	if (!shell->cmds)
		return ;
	if (!shell->cmds->args && shell->cmds->redir_count == 0
		&& !shell->cmds->next)
	{
		free_cmds(shell->cmds);
		shell->cmds = NULL;
		return ;
	}
	execute_cmds(shell);
	free_cmds(shell->cmds);
	shell->cmds = NULL;
}

char	*get_env_value(const char *name, char **envp)
{
	int		i;
	size_t	len;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	set_env_value(const char *name, const char *value, t_shell *shell)
{
	int		i;
	int		count;
	size_t	len;
	char	*entry;
	char	*tmp;
	char	**new_envp;

	len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
		{
			tmp = ft_strjoin(name, "=");
			entry = ft_strjoin(tmp, value);
			free(tmp);
			free(shell->envp[i]);
			shell->envp[i] = entry;
			return (0);
		}
		i++;
	}
	count = envp_count(shell->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (-1);
	i = 0;
	while (i < count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	tmp = ft_strjoin(name, "=");
	entry = ft_strjoin(tmp, value);
	free(tmp);
	new_envp[count] = entry;
	new_envp[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

char	**copy_envp(char **envp)
{
	int		count;
	int		i;
	char	**copy;

	count = envp_count(envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

int	envp_count(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

static int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}

char	*find_executable(const char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (is_executable(full_path))
		{
			i = 0;
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (NULL);
}

void	print_error(const char *prefix, const char *msg)
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
}
