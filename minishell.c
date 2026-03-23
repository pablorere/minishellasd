#include "minishell.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t	g_signal = 0;

typedef struct s_history
{
	char				*line;
	struct s_history	*next;
}	t_history;

static t_history	*g_history = NULL;

static void	handle_sigint(int signal_number)
{
	g_signal = signal_number;
	write(STDOUT_FILENO, "\n", 1);
}

static void	setup_signals(void)
{
	struct sigaction	action;

	action.sa_handler = handle_sigint;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static size_t	str_len(const char *str)
{
	size_t	len;

	if (str == NULL)
		return (0);
	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

static char	*str_dup(const char *str)
{
	size_t	len;
	size_t	index;
	char	*copy;

	len = str_len(str);
	copy = malloc(len + 1);
	if (copy == NULL)
		return (NULL);
	index = 0;
	while (index < len)
	{
		copy[index] = str[index];
		index++;
	}
	copy[len] = '\0';
	return (copy);
}

static void	history_add(const char *line)
{
	t_history	*node;
	t_history	*tail;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return ;
	node->line = str_dup(line);
	if (node->line == NULL)
	{
		free(node);
		return ;
	}
	node->next = NULL;
	if (g_history == NULL)
	{
		g_history = node;
		return ;
	}
	tail = g_history;
	while (tail->next != NULL)
		tail = tail->next;
	tail->next = node;
}

static void	history_clear(void)
{
	t_history	*next;

	while (g_history != NULL)
	{
		next = g_history->next;
		free(g_history->line);
		free(g_history);
		g_history = next;
	}
}

static char	*read_line(const char *prompt)
{
	char	*line;
	char	buffer;
	size_t	capacity;
	size_t	length;
	ssize_t	bytes;

	if (prompt != NULL)
		write(STDOUT_FILENO, prompt, str_len(prompt));
	capacity = 64;
	length = 0;
	line = malloc(capacity);
	if (line == NULL)
		return (NULL);
	while (1)
	{
		bytes = read(STDIN_FILENO, &buffer, 1);
		if (bytes == 0)
			break ;
		if (bytes < 0)
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				line[0] = '\0';
				return (line);
			}
			free(line);
			return (NULL);
		}
		if (buffer == '\n')
			break ;
		if (length + 1 >= capacity)
		{
			char	*expanded;
			size_t	new_capacity;
			size_t	index;

			new_capacity = capacity * 2;
			expanded = malloc(new_capacity);
			if (expanded == NULL)
			{
				free(line);
				return (NULL);
			}
			index = 0;
			while (index < length)
			{
				expanded[index] = line[index];
				index++;
			}
			free(line);
			line = expanded;
			capacity = new_capacity;
		}
		line[length++] = buffer;
	}
	if (bytes == 0 && length == 0)
	{
		free(line);
		return (NULL);
	}
	line[length] = '\0';
	return (line);
}

static int	is_exit_command(const char *line)
{
	const char	*cmd;
	size_t		index;

	if (line == NULL)
		return (0);
	cmd = "exit";
	index = 0;
	while (cmd[index] != '\0' && line[index] == cmd[index])
		index++;
	if (cmd[index] != '\0')
		return (0);
	return (line[index] == '\0');
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		line = read_line("minishell$ ");
		if (line == NULL)
			break ;
		if (line[0] != '\0')
			history_add(line);
		if (is_exit_command(line))
		{
			free(line);
			break ;
		}
		free(line);
	}
	history_clear();
	return (0);
}
