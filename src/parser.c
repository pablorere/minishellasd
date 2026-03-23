#include "minishell.h"

static t_cmd	*new_cmd(void)
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

static void	add_arg(t_cmd *cmd, char *val)
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

static void	add_redir(t_cmd *cmd, char *file, int type)
{
	char	**new_redirs;
	int		*new_types;
	int		i;
	int		n;

	n = cmd->redir_count;
	new_redirs = malloc(sizeof(char *) * (n + 2));
	new_types = malloc(sizeof(int) * (n + 1));
	if (!new_redirs || !new_types)
	{
		free(new_redirs);
		free(new_types);
		return ;
	}
	i = 0;
	while (i < n)
	{
		new_redirs[i] = cmd->redirections[i];
		new_types[i] = cmd->redir_types[i];
		i++;
	}
	new_redirs[n] = file;
	new_redirs[n + 1] = NULL;
	new_types[n] = type;
	free(cmd->redirections);
	free(cmd->redir_types);
	cmd->redirections = new_redirs;
	cmd->redir_types = new_types;
	cmd->redir_count = n + 1;
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cur;
	t_token	*tok;
	int		redir_type;

	cmds = new_cmd();
	if (!cmds)
		return (NULL);
	cur = cmds;
	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_PIPE)
		{
			cur->next = new_cmd();
			if (!cur->next)
				return (cmds);
			cur = cur->next;
		}
		else if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
			|| tok->type == TOKEN_REDIR_APPEND || tok->type == TOKEN_HEREDOC)
		{
			redir_type = tok->type;
			tok = tok->next;
			if (tok && tok->type == TOKEN_WORD)
				add_redir(cur, ft_strdup(tok->value), redir_type);
		}
		else if (tok->type == TOKEN_WORD)
			add_arg(cur, ft_strdup(tok->value));
		tok = tok->next;
	}
	return (cmds);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
				free(cmds->args[i++]);
			free(cmds->args);
		}
		if (cmds->redirections)
		{
			i = 0;
			while (cmds->redirections[i])
				free(cmds->redirections[i++]);
			free(cmds->redirections);
		}
		free(cmds->redir_types);
		free(cmds);
		cmds = tmp;
	}
}
