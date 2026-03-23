/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:27:55 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:27:55 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_single_builtin(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;
	int	ret;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
	{
		shell->exit_status = 1;
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return ;
	}
	ret = run_builtin(cmd, shell);
	shell->exit_status = ret;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

static void	run_child_process(t_cmd *cmd, t_shell *shell,
	int in_fd, int *pipefd)
{
	if (cmd->next)
	{
		close(pipefd[0]);
		exec_child(cmd, shell, in_fd, pipefd[1]);
	}
	else
		exec_child(cmd, shell, in_fd, STDOUT_FILENO);
}

static int	spawn_process(t_cmd *cmd, t_shell *shell, int *in_fd, pid_t *pid)
{
	int	pipefd[2];

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (*pid == 0)
		run_child_process(cmd, shell, *in_fd, pipefd);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	return (0);
}

void	execute_cmds(t_shell *shell)
{
	t_cmd	*cmd;
	int		in_fd;
	pid_t	last_pid;
	pid_t	pid;

	cmd = shell->cmds;
	if (!cmd)
		return ;
	if (!cmd->next && cmd->args && is_builtin(cmd->args[0]))
	{
		run_single_builtin(cmd, shell);
		return ;
	}
	in_fd = STDIN_FILENO;
	last_pid = -1;
	while (cmd)
	{
		if (spawn_process(cmd, shell, &in_fd, &pid) == -1)
			return ;
		last_pid = pid;
		cmd = cmd->next;
	}
	wait_children(last_pid, shell);
}
