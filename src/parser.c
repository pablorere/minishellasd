/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:26:31 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:26:31 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redir(t_cmd *cur, t_token **tok)
{
	int	type;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (*tok && (*tok)->type == TOKEN_WORD)
		add_redir(cur, ft_strdup((*tok)->value), type);
}

static int	process_token(t_cmd **cur, t_token **tok)
{
	if ((*tok)->type == TOKEN_PIPE)
	{
		(*cur)->next = new_cmd();
		if (!(*cur)->next)
			return (0);
		*cur = (*cur)->next;
	}
	else if ((*tok)->type != TOKEN_WORD)
		handle_redir(*cur, tok);
	else
		add_arg(*cur, ft_strdup((*tok)->value));
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cur;
	t_token	*tok;

	cmds = new_cmd();
	if (!cmds)
		return (NULL);
	cur = cmds;
	tok = tokens;
	while (tok)
	{
		if (!process_token(&cur, &tok))
			return (cmds);
		if (tok)
			tok = tok->next;
	}
	return (cmds);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free_cmd_content(cmds);
		free(cmds);
		cmds = tmp;
	}
}
