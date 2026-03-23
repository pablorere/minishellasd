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
		if (!line || (ft_strncmp(line, delimiter, dlen) == 0 && line[dlen] == '\0'))
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

int	apply_redirections(t_cmd *cmd)
{
	int	i;
	int	fd;
	int	heredoc_fd;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redir_types[i] == TOKEN_REDIR_IN)
		{
			fd = open(cmd->redirections[i], O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->redirections[i], 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putchar_fd('\n', 2);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd->redir_types[i] == TOKEN_REDIR_OUT)
		{
			fd = open(cmd->redirections[i],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("minishell");
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redir_types[i] == TOKEN_REDIR_APPEND)
		{
			fd = open(cmd->redirections[i],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("minishell");
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redir_types[i] == TOKEN_HEREDOC)
		{
			heredoc_fd = handle_heredoc(cmd->redirections[i]);
			if (heredoc_fd == -1)
				return (-1);
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		i++;
	}
	return (0);
}
