/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:13:25 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:13:25 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	print_error(const char *prefix, const char *msg)
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
}
