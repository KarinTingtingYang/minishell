/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 10:18:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Prints an error message and exits the program.
 * This function prints a formatted error message to standard error
 * and then exits the program with the specified exit status.
 * @param command The command or context where the error occurred.
 * @param message The error message to display.
 * @param exit_status The exit status code to return.
 * @return This function does not return; it exits the program.
 */
void	ft_error_and_exit(char *command, char *message, int exit_status)
{
	ft_error(command, message);
	exit(exit_status);
}

/**
 * @brief Prints a formatted error message to standard error.
 * This function prints an error message in the format:
 * "minishell: <command>: <message>\n" to standard error.
 * If the command is NULL or an empty string, it omits the command part.
 * @param command The command or context where the error occurred.
 * @param message The error message to display.
 */
void	ft_error(char *command, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command && *command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/**
 * @brief Prints an error message with an argument to standard error.
 * This function prints an error message in the format:
 * "minishell: <command>: <arg>: <message>\n" to standard
 * error. If the command or arg is NULL or an empty string,
 * it omits those parts accordingly.
 * @param command The command or context where the error occurred.
 * @param arg The argument related to the error.
 * @param message The error message to display.
 * @return This function does not return; it exits the program.
 */
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

/**
 * @brief Prints an error message with a backticked argument to standard error.
 * This function prints an error message in the format:
 * "minishell: <command>: `<arg>`: <message>\n" to standard
 * error. If the command or arg is NULL or an empty string,
 * it omits those parts accordingly.
 * @param command The command or context where the error occurred.
 * @param arg The argument related to the error.
 * @param message The error message to display.
 * @return This function does not return; it exits the program.
 */
void	error_with_backticked_arg(const char *cmd, const char *arg,
										const char *msg)
{
	char	*tmp;
	char	*backticked;

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
		ft_error_with_arg((char *)cmd, (char *)arg, (char *)msg);
	}
}
