/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:04:21 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 11:13:53 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

char	**find_path_dirs(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path_value = envp[i] + 5;
	return (ft_split(path_value, ':'));
}
