/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:34:52 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 08:33:44 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*combine_cmd_path(const char *path_dir, const char *cmd)
{
	size_t	length;
	char	*full_cmd_path;

	length = ft_strlen(path_dir) + 1 + ft_strlen(cmd) + 1;
	full_cmd_path = malloc(length);
	if (full_cmd_path == NULL)
		return (NULL);
	ft_strlcpy(full_cmd_path, path_dir, length);
	ft_strlcat(full_cmd_path, "/", length);
	ft_strlcat(full_cmd_path, cmd, length);
	return (full_cmd_path);
}

char	*find_full_cmd_path(char *cmd, char **path_dirs)
{
	char	*full_cmd_path;
	int		i;

	if (!cmd || *cmd == '\0' || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (path_dirs != NULL && path_dirs[i] != NULL)
	{
		full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
		if (full_cmd_path == NULL)
			error_exit("malloc");
		if (access(full_cmd_path, X_OK) == 0)
			return (full_cmd_path);
		free(full_cmd_path);
		i++;
	}
	return (ft_strdup(cmd));
}

char	**find_path_dirs(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path_value = envp[i] + 5;
	return (ft_split(path_value, ':'));
}
