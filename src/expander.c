/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:27:15 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:27:15 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_string(const char *str, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (*str)
	{
		if (*str == '\'')
			result = handle_single_quote(&str, result);
		else if (*str == '"')
			result = handle_double_quote(&str, shell, result);
		else if (*str == '$' && is_valid_start(*(str + 1)))
			result = expand_dollar(&str, shell, result);
		else
		{
			result = append_char(result, *str);
			str++;
		}
	}
	return (result);
}

void	expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*tok;
	char	*expanded;
	int		has_quotes;

	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD)
		{
			has_quotes = 0;
			if (ft_strchr(tok->value, '\'') || ft_strchr(tok->value, '"'))
				has_quotes = 1;
			expanded = expand_string(tok->value, shell);
			free(tok->value);
			tok->value = expanded;
			if (!has_quotes && (!expanded || !*expanded))
				tok->type = -1;
		}
		tok = tok->next;
	}
}
