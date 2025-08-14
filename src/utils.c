/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 13:53:20 by makhudon         ###   ########.fr       */
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

char *ft_strjoin_free(char *s1, char *s2)
{
    char *joined = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (joined);
}


void	skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
		(*s)++;
}

void print_array(char **array)
{
	int i = 0;
	if (!array)
	{
		ft_putstr_fd("NULL\n", STDOUT_FILENO);
		return;
	}
	while (array[i])
	{
		printf("Array[%d]: %s\n", i, array[i]);
		i++;
	}
}

void print_tokens(t_token **tokens)
{
	int i = 0;
	if (!tokens)
	{
		ft_putstr_fd("NULL\n", STDOUT_FILENO);
		return;
	}
	while (tokens[i])
	{
		printf("Token %d: value='%s', quote=%d\n", i, tokens[i]->value, tokens[i]->quote);
		i++;
	}
}
