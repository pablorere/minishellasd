#include "minishell.h"

static t_token	*new_token(char *value, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = value;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

static void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '\0');
}

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

t_token	*tokenize(const char *line)
{
	t_token	*tokens;
	t_token	*tok;
	char	*val;

	tokens = NULL;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (!*line)
			break ;
		if (*line == '|')
		{
			tok = new_token(ft_strdup("|"), TOKEN_PIPE);
			token_add_back(&tokens, tok);
			line++;
		}
		else if (*line == '>' && *(line + 1) == '>')
		{
			tok = new_token(ft_strdup(">>"), TOKEN_REDIR_APPEND);
			token_add_back(&tokens, tok);
			line += 2;
		}
		else if (*line == '<' && *(line + 1) == '<')
		{
			tok = new_token(ft_strdup("<<"), TOKEN_HEREDOC);
			token_add_back(&tokens, tok);
			line += 2;
		}
		else if (*line == '>')
		{
			tok = new_token(ft_strdup(">"), TOKEN_REDIR_OUT);
			token_add_back(&tokens, tok);
			line++;
		}
		else if (*line == '<')
		{
			tok = new_token(ft_strdup("<"), TOKEN_REDIR_IN);
			token_add_back(&tokens, tok);
			line++;
		}
		else
		{
			val = read_word(&line);
			if (val)
			{
				tok = new_token(val, TOKEN_WORD);
				token_add_back(&tokens, tok);
			}
		}
	}
	return (tokens);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
