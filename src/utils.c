/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 11:13:58 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	error_msg_exit(const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

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

void	free_split(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
