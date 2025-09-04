/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:21:01 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/04 12:13:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is a valid -n flag for echo.
 * Valid -n flags are of the form -n, -nn, -nnn, etc.
 *
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
static int	is_valid_n_flag(char *str)
{
	int	i;

	if (str == NULL || str[0] != '-')
		return (0);
	if (str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

/**
 * @brief Implements the 'echo' built-in command.
 * Prints the arguments to standard output, separated by spaces.
 * Supports the -n flag to omit the trailing newline.
 *
 * @param args The command arguments. args[0] is "echo", args[1..n] are the
 * strings to print.
 * @param env_list The linked list of environment variables (not used here).
 */
void	run_echo(char **args, t_env_var *env_list)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	(void)env_list;
	while (args[i] != NULL && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i] != NULL)
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
