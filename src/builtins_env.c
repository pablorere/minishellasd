/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:23:01 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:27:01 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_variable(const char *key, t_shell *shell)
{
	int		j;
	int		k;
	size_t	len;

	len = ft_strlen(key);
	j = 0;
	while (shell->envp[j])
	{
		if (ft_strncmp(shell->envp[j], key, len) == 0
			&& (shell->envp[j][len] == '=' || shell->envp[j][len] == '\0'))
		{
			free(shell->envp[j]);
			k = j;
			while (shell->envp[k])
			{
				shell->envp[k] = shell->envp[k + 1];
				k++;
			}
			return ;
		}
		j++;
	}
}

int	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		unset_variable(cmd->args[i], shell);
		i++;
	}
	return (0);
}

static int	print_export_env(t_shell *shell)
{
	int	i;

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

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	if (!cmd->args[1])
	{
		print_export_env(shell);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (process_export_arg(cmd->args[i], shell))
			status = 1;
		i++;
	}
	return (status);
}
