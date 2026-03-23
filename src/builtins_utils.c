/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:23:01 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:23:01 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_numeric_arg(const char *arg)
{
	int	i;

	if (!arg || !*arg)
		return (0);
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_eq(char *arg, char *eq, t_shell *shell)
{
	char	*name;
	char	*val;

	name = ft_substr(arg, 0, (unsigned int)(eq - arg));
	if (is_valid_env_name(name))
	{
		val = ft_strdup(eq + 1);
		set_env_value(name, val, shell);
		free(val);
		free(name);
		return (0);
	}
	free(name);
	return (1);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		if (handle_export_eq(arg, eq, shell) == 0)
			return (0);
	}
	else if (is_valid_env_name(arg))
	{
		set_env_value(arg, "", shell);
		return (0);
	}
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}
