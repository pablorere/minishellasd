/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 15:45:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 15:45:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_signal_error(int status)
{
	if (WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	else if (WTERMSIG(status) == SIGSEGV)
		ft_putstr_fd("Segmentation fault (core dumped)\n", 2);
	else if (WTERMSIG(status) == SIGPIPE)
		ft_putstr_fd(" Broken pipe\n", 2);
}

void	wait_children(pid_t last_pid, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (WIFSIGNALED(status))
			print_signal_error(status);
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	shell->exit_status = last_status;
}
