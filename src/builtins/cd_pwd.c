/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:45:26 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/25 10:06:34 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Implements the 'cd' built-in command.
 *
 * Changes the current working directory to the specified path.
 * If no path is provided, it changes to the home directory.
 * @param args The arguments array. Should contain the target
 *             directory as args[1].
 * @return Returns 0 on success, 1 on failure.
 */
int	run_cd(char **args, t_env_var *env_list)
{
	char		*path;
	t_env_var	*home_var;

	if (args[1] == NULL)
	{
		home_var = find_env_var("HOME", env_list);
		if (home_var == NULL || home_var->value == NULL
			|| *(home_var->value) == '\0')
		{
			ft_error("cd", "HOME not set");
			return (1);
		}
		path = home_var->value;
	}
	else
	{
		path = args[1];
	}
	if (chdir(path) != 0)
	{
		ft_error_with_arg("cd", path, strerror(errno));
		return (1);
	}
	return (0);
}

/**
 * @brief Implements the 'pwd' built-in command.
 * Prints the current working directory.
 * @return Returns 0 on success, 1 on failure.
 */
int	run_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	else
	{
		ft_error("pwd", strerror(errno));
		return (1);
	}
}
