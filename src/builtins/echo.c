/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:21:01 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/28 12:43:17 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/minishell.h"
// /**
//  * @brief Executes the built-in echo command.
//  * 
//  * Prints all arguments separated by spaces and ends with a newline.
//  * This basic version ignores any options like -n.
//  * @param args Array of strings, args[0] = "echo", args[1..] = words
//  * to print.
//  */
// void run_echo(char **args)
// {
// 	int i = 1;

// 	while (args[i])
// 	{
// 		ft_putstr_fd(args[i], STDOUT_FILENO);
// 		if (args[i + 1])
// 			ft_putchar_fd(' ', STDOUT_FILENO);
// 		i++;
// 	}
// 	ft_putchar_fd('\n', STDOUT_FILENO);
// }

#include "../includes/minishell.h"

/**
 * @brief Executes the built-in echo command with support for -n.
 *
 * Handles echo with the -n option to suppress newline.
 */
void	run_echo(char **args)
{
	int	i = 1;
	int	newline = 1;

	// Handle -n option
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
