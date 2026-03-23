#include "minishell.h"

static int	count_cmds(t_cmd *cmds)
{
	int	n;

	n = 0;
	while (cmds)
	{
		n++;
		cmds = cmds->next;
	}
	return (n);
}

static void	exec_child(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	*path;

	setup_signals_child();
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (apply_redirections(cmd) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, shell));
	path = find_executable(cmd->args[0], shell->envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, shell->envp);
	perror("minishell");
	free(path);
	exit(1);
}

static void	run_single_builtin(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;
	int	ret;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
	{
		shell->exit_status = 1;
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return ;
	}
	ret = run_builtin(cmd, shell);
	shell->exit_status = ret;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

static void	wait_children(pid_t last_pid, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	shell->exit_status = last_status;
}

void	execute_cmds(t_shell *shell)
{
	t_cmd	*cmd;
	int		n;
	int		pipefd[2];
	int		in_fd;
	pid_t	pid;
	pid_t	last_pid;

	cmd = shell->cmds;
	n = count_cmds(cmd);
	if (n == 1 && cmd->args && is_builtin(cmd->args[0]))
	{
		run_single_builtin(cmd, shell);
		return ;
	}
	in_fd = STDIN_FILENO;
	last_pid = -1;
	while (cmd)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		if (cmd->next)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == 0)
		{
			if (cmd->next)
			{
				close(pipefd[0]);
				exec_child(cmd, shell, in_fd, pipefd[1]);
			}
			else
				exec_child(cmd, shell, in_fd, STDOUT_FILENO);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			in_fd = pipefd[0];
		}
		last_pid = pid;
		cmd = cmd->next;
	}
	wait_children(last_pid, shell);
}
