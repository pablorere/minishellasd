#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

int	run_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (builtin_echo(cmd));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (builtin_cd(cmd, shell));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (builtin_export(cmd, shell));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (builtin_unset(cmd, shell));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (builtin_env(shell));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (builtin_exit(cmd, shell));
	return (1);
}

int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	buf[4096];

	if (!cmd->args[1])
	{
		path = get_env_value("HOME", shell->envp);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	if (getcwd(buf, sizeof(buf)))
		set_env_value("PWD", buf, shell);
	return (0);
}

int	builtin_pwd(void)
{
	char	buf[4096];

	if (getcwd(buf, sizeof(buf)))
	{
		ft_putstr_fd(buf, 1);
		ft_putchar_fd('\n', 1);
		return (0);
	}
	perror("pwd");
	return (1);
}

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*eq;
	char	*name;
	char	*val;

	i = 1;
	if (!cmd->args[1])
	{
		i = 0;
		while (shell->envp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(shell->envp[i], 1);
			ft_putchar_fd('\n', 1);
			i++;
		}
		return (0);
	}
	while (cmd->args[i])
	{
		eq = ft_strchr(cmd->args[i], '=');
		if (eq)
		{
			name = ft_substr(cmd->args[i], 0,
					(unsigned int)(eq - cmd->args[i]));
			val = ft_strdup(eq + 1);
			set_env_value(name, val, shell);
			free(name);
			free(val);
		}
		else
			set_env_value(cmd->args[i], "", shell);
		i++;
	}
	return (0);
}

int	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	size_t	len;

	i = 1;
	while (cmd->args[i])
	{
		len = ft_strlen(cmd->args[i]);
		j = 0;
		while (shell->envp[j])
		{
			if (ft_strncmp(shell->envp[j], cmd->args[i], len) == 0
				&& (shell->envp[j][len] == '=' || shell->envp[j][len] == '\0'))
			{
				free(shell->envp[j]);
				k = j;
				while (shell->envp[k])
				{
					shell->envp[k] = shell->envp[k + 1];
					k++;
				}
			}
			else
				j++;
		}
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	int	code;

	ft_putstr_fd("exit\n", 1);
	code = shell->exit_status;
	if (cmd->args[1])
		code = ft_atoi(cmd->args[1]) % 256;
	shell->exit_status = code;
	cleanup_shell(shell);
	exit(code);
}
