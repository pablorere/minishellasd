/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:26:10 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:26:10 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;
	size_t	dlen;

	if (pipe(pipefd) == -1)
		return (-1);
	dlen = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strncmp(line, delimiter, dlen)
				== 0 && line[dlen] == '\0'))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

static int	handle_redir_in(char *file, int type)
{
	int	fd;

	if (type == TOKEN_HEREDOC)
		fd = handle_heredoc(file);
	else
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			return (-1);
		}
	}
	if (fd == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_out(char *file, int type)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (type == TOKEN_REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	apply_redirections(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redir_types[i] == TOKEN_REDIR_IN
			|| cmd->redir_types[i] == TOKEN_HEREDOC)
		{
			if (handle_redir_in(cmd->redirections[i],
					cmd->redir_types[i]) == -1)
				return (-1);
		}
		else if (cmd->redir_types[i] == TOKEN_REDIR_OUT
			|| cmd->redir_types[i] == TOKEN_REDIR_APPEND)
		{
			if (handle_redir_out(cmd->redirections[i],
					cmd->redir_types[i]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
