/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:05:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 16:05:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->redir_types = NULL;
	cmd->redir_count = 0;
	cmd->pipe_in = -1;
	cmd->pipe_out = -1;
	cmd->next = NULL;
	return (cmd);
}

void	add_arg(t_cmd *cmd, char *val)
{
	char	**new_args;
	int		i;
	int		count;

	count = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = val;
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

static void	copy_arrays(t_cmd *cmd, char **nr, int *nt)
{
	int	i;

	i = -1;
	while (++i < cmd->redir_count)
	{
		nr[i] = cmd->redirections[i];
		nt[i] = cmd->redir_types[i];
	}
}

void	add_redir(t_cmd *cmd, char *file, int type)
{
	char	**nr;
	int		*nt;
	int		n;

	n = cmd->redir_count;
	nr = malloc(sizeof(char *) * (n + 2));
	nt = malloc(sizeof(int) * (n + 1));
	if (!nr || !nt)
	{
		free(nr);
		free(nt);
		return ;
	}
	copy_arrays(cmd, nr, nt);
	nr[n] = file;
	nr[n + 1] = NULL;
	nt[n] = type;
	free(cmd->redirections);
	free(cmd->redir_types);
	cmd->redirections = nr;
	cmd->redir_types = nt;
	cmd->redir_count = n + 1;
}

void	free_cmd_content(t_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->redirections)
	{
		i = 0;
		while (cmd->redirections[i])
			free(cmd->redirections[i++]);
		free(cmd->redirections);
	}
	free(cmd->redir_types);
}
