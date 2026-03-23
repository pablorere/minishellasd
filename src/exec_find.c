/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-s <ppaula-s@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:25:00 by ppaula-s          #+#    #+#             */
/*   Updated: 2026/03/23 16:25:00 by ppaula-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*check_path_entry(char *dir, const char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (full_path && is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_executable(const char *cmd, char **envp)
{
	char	**paths;
	char	*res;
	char	*path_env;
	int		i;

	if (!cmd || !*cmd || ft_strchr(cmd, '/'))
	{
		if (cmd && *cmd && ft_strchr(cmd, '/'))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	res = NULL;
	while (paths[++i] && !res)
		res = check_path_entry(paths[i], cmd);
	free_paths(paths);
	return (res);
}
