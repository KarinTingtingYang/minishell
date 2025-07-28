/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:00:14 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/28 09:36:09 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes the built-in echo command.
 * 
 * Prints all arguments separated by spaces and ends with a newline.
 * This basic version ignores any options like -n.
 * @param args Array of strings, args[0] = "echo", args[1..] = words
 * to print.
 */
void run_echo(char **args)
{
	int i = 1;

	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int run_builtin(char **args)
{
	if (args == NULL || args[0] == NULL)
		return (0);
	if (ft_strncmp(args[0], "echo", ft_strlen("echo") + 1) == 0)
	{
		run_echo(args);
		return (1);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "echo", 5) == 0);
}
