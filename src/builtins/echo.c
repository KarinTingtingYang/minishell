/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 11:21:01 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/19 10:18:36 by tiyang        ########   odam.nl         */
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
// void	run_echo(char **args)
// {
// 	int	i;
// 	int	newline;
	
// 	i = 1;
// 	newline = 1;
// 	if (args[i]  && ft_strncmp(args[i], "-n", 3) == 0)
// 	{
// 		newline = 0;
// 		i++;
// 	}
// 	while (args[i] != NULL)
// 	{
// 		ft_putstr_fd(args[i], STDOUT_FILENO);
// 		if (args[i + 1] != NULL)
// 			ft_putchar_fd(' ', STDOUT_FILENO);
// 		i++;
// 	}
// 	if (newline)
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// }

static int is_valid_n_flag(char *s)
{
    int i = 1;

    if (!s || s[0] != '-')
        return (0);
    while (s[i] == 'n')
        i++;
    if (s[i] == '\0')
        return (1);
    return (0);
}

// void    run_echo(char **args)
void run_echo(char **args, t_env_var *env_list)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1;
	(void)env_list;
	//t_quote_type quote = NO_QUOTE; 

    // A standard echo built-in should handle multiple -n flags (e.g., echo -n -n -n).
    // This loop checks for and skips all valid -n flags at the beginning of the arguments.
    while (args[i] && is_valid_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    // Print the remaining arguments, separated by a space.
    // while (args[i] != NULL)
    // {
    //     ft_putstr_fd(args[i], STDOUT_FILENO);
    //     if (args[i + 1] != NULL)
    //         ft_putchar_fd(' ', STDOUT_FILENO);
    //     i++;
    // }

	while (args[i] != NULL)
	{
		//char *expanded = expand_variables(args[i], env_list, 0, quote);
		//ft_putstr_fd(expanded, STDOUT_FILENO);
		//free(expanded); // let the expander handle expansion
		ft_putstr_fd(args[i], STDOUT_FILENO);

		if (args[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
    
    // Add a newline unless the -n flag(s) was/were present.
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
}