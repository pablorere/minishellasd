/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:27:06 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:27:06 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*skip_quoted(const char *s)
{
	char	quote;

	quote = *s++;
	while (*s && *s != quote)
		s++;
	if (*s == quote)
		s++;
	return (s);
}

static char	*read_word(const char **s)
{
	const char	*start;
	const char	*p;
	size_t		len;
	char		*result;

	start = *s;
	p = *s;
	while (*p && !ft_isspace(*p) && !is_special(*p))
	{
		if (*p == '\'' || *p == '"')
			p = skip_quoted(p);
		else
			p++;
	}
	len = p - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, start, len);
	result[len] = '\0';
	*s = p;
	return (result);
}

static t_token	*get_ret(const char **s, char *v, int type, int adv)
{
	*s += adv;
	return (new_token(ft_strdup(v), type));
}

static t_token	*get_op(const char **s)
{
	if (**s == '|')
		return (get_ret(s, "|", TOKEN_PIPE, 1));
	if (**s == '>' && *(*s + 1) == '>')
		return (get_ret(s, ">>", TOKEN_REDIR_APPEND, 2));
	if (**s == '<' && *(*s + 1) == '<')
		return (get_ret(s, "<<", TOKEN_HEREDOC, 2));
	if (**s == '>')
		return (get_ret(s, ">", TOKEN_REDIR_OUT, 1));
	if (**s == '<')
		return (get_ret(s, "<", TOKEN_REDIR_IN, 1));
	return (NULL);
}

t_token	*tokenize(const char *line)
{
	t_token	*h;
	t_token	*tok;

	h = NULL;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (!*line)
			break ;
		tok = get_op(&line);
		if (!tok)
		{
			tok = new_token(read_word(&line), TOKEN_WORD);
			if (!tok->value)
				ft_putstr_fd("check_alloc\n", 2);
		}
		if (tok)
			token_add_back(&h, tok);
	}
	return (h);
}
