/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:45:26 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/30 08:31:53 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Implements the 'cd' built-in command.
 * Changes the current working directory.
 * @param args The arguments array, where args[1] is the path.
 * @return Returns 0 on success, 1 on failure.
 */
int	builtin_cd(char **args)
{
	if (args[1] == NULL)
	{
		// In a real shell, this would go to the HOME directory.
		// For this implementation, we'll treat it as an error.
		// as the subject requires "cd with only a relative or absolute path"
		ft_putstr_fd("minishell: cd: missing argument\n", STDERR_FILENO);
		return (1);

		/* //to mimic the real shell behavior, use the below code instead:
		path = getenv("HOME");
		if (path == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
		*/
		
	}
	if (chdir(args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(args[1]);
		return (1);
	}
	return (0);
}

/**
 * @brief Implements the 'pwd' built-in command.
 * Prints the current working directory to standard output.
 * @return Returns 0 on success, 1 on failure.
 */
int	builtin_pwd(void)
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
		perror("minishell: pwd");
		return (1);
	}
}
