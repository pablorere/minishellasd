/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:15:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 16:15:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_single_quote(const char **str, char *result)
{
	(*str)++;
	while (**str && **str != '\'')
	{
		result = append_char(result, **str);
		(*str)++;
	}
	if (**str == '\'')
		(*str)++;
	return (result);
}

char	*handle_double_quote(const char **str, t_shell *shell, char *result)
{
	(*str)++;
	while (**str && **str != '"')
	{
		if (**str == '$' && is_valid_start(*(*str + 1)))
			result = expand_dollar(str, shell, result);
		else
		{
			result = append_char(result, **str);
			(*str)++;
		}
	}
	if (**str == '"')
		(*str)++;
	return (result);
}
