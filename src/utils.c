/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:16:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// void	error_exit(const char *msg)
// {
// 	perror(msg);
// 	exit(EXIT_FAILURE);
// }

// void	error_msg_exit(const char *msg)
// {
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd((char *)msg, STDERR_FILENO);
// 	ft_putstr_fd("\n", STDERR_FILENO);
// 	exit(EXIT_FAILURE);
// }

// Exits with a specified status after printing an error.
void ft_error_and_exit(char *command, char *message, int exit_status)
{
    ft_error(command, message);
    exit(exit_status);
}

// A central function to print errors.
void ft_error(char *command, char *message)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command && *command) // Only print if cmd is not empty
    {
        ft_putstr_fd(command, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    // ft_putstr_fd(command, STDERR_FILENO);
    // ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(message, STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

void	ft_error_with_arg(char *command, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command && *command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg && *arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/* Prints: minishell: <cmd>: `arg': <msg> */
void error_with_backticked_arg(const char *cmd, const char *arg, const char *msg)
{
	char *tmp;
	char *backticked;

	tmp = ft_strjoin("`", (char *)arg);
	backticked = NULL;
	if (tmp)
	{
		backticked = ft_strjoin(tmp, "'");
		free(tmp);
	}
	if (backticked)
	{
		ft_error_with_arg((char *)cmd, backticked, (char *)msg);
		free(backticked);
	}
	else
	{
		/* fallback if allocation failed */
		ft_error_with_arg((char *)cmd, (char *)arg, (char *)msg);
	}
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
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void	skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
		(*s)++;
}
