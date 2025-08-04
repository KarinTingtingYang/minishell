/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:21:01 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/04 10:25:00 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Implements the 'echo' built-in command.
 *
 * This function prints the arguments to standard output. If the first argument
 * is "-n", it does not print a newline at the end.
 * @param args The arguments array. Should contain the strings to echo.
 */
void	run_echo(char **args)
{
	int	i;
	int	newline;
	
	i = 1;
	newline = 1;
	if (args[i]  && ft_strncmp(args[i], "-n", 3) == 0)
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
