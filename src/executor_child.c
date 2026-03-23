/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 15:30:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 15:30:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_enoexec_exec(char *path, t_cmd *cmd, t_shell *shell)
{
	char	**new_args;
	int		i;
	int		argc;

	argc = 0;
	while (cmd->args[argc])
		argc++;
	new_args = malloc(sizeof(char *) * (argc + 2));
	if (new_args)
	{
		new_args[0] = "sh";
		new_args[1] = path;
		i = 1;
		while (cmd->args[i])
		{
			new_args[i + 1] = cmd->args[i];
			i++;
		}
		new_args[i + 1] = NULL;
		execve("/bin/sh", new_args, shell->envp);
		free(new_args);
	}
}

void	handle_enoexec(char *path, t_cmd *cmd, t_shell *shell)
{
	if (errno == ENOEXEC)
		handle_enoexec_exec(path, cmd, shell);
}

void	handle_exec_error(char *path, char *cmd_name)
{
	struct stat	st;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	if (errno == EACCES)
	{
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(": Is a directory\n", 2);
			free(path);
			exit(126);
		}
	}
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	free(path);
	exit(1);
}

void	handle_cmd_not_found(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	exec_child(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	*path;

	setup_signals_child();
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (apply_redirections(cmd) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, shell));
	path = find_executable(cmd->args[0], shell->envp);
	if (!path)
		handle_cmd_not_found(cmd->args[0]);
	execve(path, cmd->args, shell->envp);
	handle_enoexec(path, cmd, shell);
	handle_exec_error(path, cmd->args[0]);
}
