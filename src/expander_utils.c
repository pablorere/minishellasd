/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:00:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 16:00:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_start(char c)
{
	if (c == '?')
		return (1);
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}

char	*append_char(char *result, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(result, buf);
	free(result);
	return (tmp);
}

static char	*get_var_name(const char *s, int *len)
{
	int	i;

	i = 0;
	if (s[i] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	*len = i;
	if (i == 0)
		return (ft_strdup(""));
	return (ft_substr(s, 0, i));
}

static char	*expand_var(const char *name, t_shell *shell)
{
	char	*val;

	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(shell->exit_status));
	val = get_env_value(name, shell->envp);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*expand_dollar(const char **str, t_shell *shell, char *result)
{
	char	*var_name;
	char	*var_val;
	char	*tmp;
	int		len;

	(*str)++;
	if (!**str)
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		return (tmp);
	}
	var_name = get_var_name(*str, &len);
	*str += len;
	var_val = expand_var(var_name, shell);
	free(var_name);
	tmp = ft_strjoin(result, var_val);
	free(result);
	free(var_val);
	return (tmp);
}
