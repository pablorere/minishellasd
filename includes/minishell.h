/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:12:31 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 14:12:31 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

extern int	g_signal;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	char			**redirections;
	int				*redir_types;
	int				redir_count;
	int				pipe_in;
	int				pipe_out;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char		**envp;
	int			exit_status;
	t_token		*tokens;
	t_cmd		*cmds;
}	t_shell;

/* signals.c */
void	setup_signals(void);
void	setup_signals_child(void);

/* lexer.c */
t_token	*tokenize(const char *line);
void	free_tokens(t_token *tokens);

/* lexer_utils.c */
t_token	*new_token(char *value, int type);
void	token_add_back(t_token **lst, t_token *new);
int		is_special(char c);

/* parser.c */
t_cmd	*parse_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);

/* parser_utils.c */
t_cmd	*new_cmd(void);
void	add_arg(t_cmd *cmd, char *val);
void	add_redir(t_cmd *cmd, char *file, int type);
void	free_cmd_content(t_cmd *cmd);

/* expander.c */
void	expand_tokens(t_token *tokens, t_shell *shell);
char	*expand_string(const char *str, t_shell *shell);

/* expander_utils.c */
int		is_valid_start(char c);
char	*append_char(char *result, char c);
char	*expand_dollar(const char **str, t_shell *shell, char *result);

/* expander_quotes.c */
char	*handle_single_quote(const char **str, char *result);
char	*handle_double_quote(const char **str, t_shell *shell, char *result);

/* executor.c */
void	execute_cmds(t_shell *shell);

/* executor_child.c */
void	exec_child(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd);

/* executor_utils.c */
void	wait_children(pid_t last_pid, t_shell *shell);

/* builtins.c */
int		is_builtin(const char *cmd);
int		run_builtin(t_cmd *cmd, t_shell *shell);
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd, t_shell *shell);
int		builtin_pwd(void);
int		builtin_export(t_cmd *cmd, t_shell *shell);
int		builtin_unset(t_cmd *cmd, t_shell *shell);
int		builtin_env(t_shell *shell);
int		builtin_exit(t_cmd *cmd, t_shell *shell);

/* builtins_utils.c */
int		is_valid_env_name(const char *name);
int		is_numeric_arg(const char *arg);
int		process_export_arg(char *arg, t_shell *shell);

/* redirections.c */
int		apply_redirections(t_cmd *cmd);
int		handle_heredoc(const char *delimiter);

/* utils.c */
void	init_shell(t_shell *shell, char **envp);
void	cleanup_shell(t_shell *shell);
void	process_line(t_shell *shell, char *line);
char	*find_executable(const char *cmd, char **envp);
char	*get_env_value(const char *name, char **envp);
int		set_env_value(const char *name, const char *value, t_shell *shell);
char	**copy_envp(char **envp);
void	free_envp(char **envp);
int		envp_count(char **envp);
void	print_error(const char *prefix, const char *msg);

#endif
