/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:45:26 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/22 08:37:46 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Implements the 'cd' built-in command.
 *
 * Changes the current working directory to the specified path.
 * If no path is provided, it changes to the home directory.
 * @param args The arguments array. Should contain the target directory as args[1].
 * @return Returns 0 on success, 1 on failure.
 */
int	run_cd(char **args, t_env_var *env_list)
{
	char	*path;
	t_env_var	*home_var;

	// Case 1: If no path is provided, it changes to the home directory.
	if (args[1] == NULL)
	{
		home_var = find_env_var("HOME", env_list);
		if (home_var == NULL || home_var->value == NULL || *(home_var->value) == '\0')
		{
			// ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO); // DEBUG: Print error if HOME is not set
			ft_error("cd", "HOME not set");
			return (1);
		}
		path = home_var->value;
		
	}
	// Case 2: 'cd' with an argument (e.g., "cd /tmp")
	else
	{
		path = args[1];
	}
	// Execute the change of directory
	if (chdir(path) != 0)
	{
		// ft_putstr_fd("minishell: cd: ", STDERR_FILENO); // DEBUG: Print error if chdir fails
		// perror(path);
		// ft_error("cd", strerror(errno));
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
		// perror("minishell: pwd"); // DEBUG: Print error if getcwd fails
		ft_error("pwd", strerror(errno));
		return (1);
	}
}