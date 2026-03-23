/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:20:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 16:20:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_env_str(const char *name, const char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

static int	add_new_env(const char *n, const char *value, t_shell *shell)
{
	int		count;
	int		i;
	char	**new_envp;

	count = envp_count(shell->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (-1);
	i = -1;
	while (++i < count)
		new_envp[i] = shell->envp[i];
	new_envp[count] = create_env_str(n, value);
	new_envp[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

/* n = name size = strlen*/
int	set_env_value(const char *n, const char *value, t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->envp && shell->envp[++i])
	{
		if (ft_strncmp(shell->envp[i], n, size(n)) == 0
			&& shell->envp[i][size(n)] == '=')
		{
			free(shell->envp[i]);
			shell->envp[i] = create_env_str(n, value);
			return (0);
		}
	}
	return (add_new_env(n, value, shell));
}
