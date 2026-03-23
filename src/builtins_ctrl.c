/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_ctrl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:23:01 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:23:01 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_numeric_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	cleanup_shell(shell);
	exit(2);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	int	code;

	ft_putstr_fd("exit\n", 1);
	if (!cmd->args[1])
	{
		code = shell->exit_status;
		cleanup_shell(shell);
		exit(code);
	}
	if (!is_numeric_arg(cmd->args[1]))
		exit_numeric_error(cmd->args[1], shell);
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	code = ft_atoi(cmd->args[1]);
	shell->exit_status = code;
	cleanup_shell(shell);
	exit(code);
}

static int	get_cd_path(t_cmd *cmd, t_shell *shell, char **path)
{
	if (!cmd->args[1])
	{
		*path = get_env_value("HOME", shell->envp);
		if (!*path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (cmd->args[2])
	{
		print_error("Error", "too many arguments");
		return (1);
	}
	else
		*path = cmd->args[1];
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	buf[4096];

	if (get_cd_path(cmd, shell, &path))
		return (1);
	if (chdir(path) == -1)
	{
		print_error("cd", strerror(errno));
		return (1);
	}
	if (getcwd(buf, sizeof(buf)))
		set_env_value("PWD", buf, shell);
	return (0);
}
