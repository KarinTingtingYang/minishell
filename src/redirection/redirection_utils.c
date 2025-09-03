/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:10:49 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/03 15:55:00 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Processes an output file for redirection.
 *
 * This function attempts to open the specified output file with the
 * appropriate flags based on whether appending is requested. If the
 * file cannot be opened or created, it prints an error message.
 *
 * @param output_file The name of the output file to process.
 * @param is_append A flag indicating whether to append (1)
 *                  or truncate (0) the file.
 * @return 0 on success, -1 on failure.
 */
int	process_output_file(char *output_file, int is_append)
{
	int	fd_out;

	if (is_append)
		fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		ft_error(output_file, strerror(errno));
		return (-1);
	}
	close(fd_out);
	return (0);
}

/**
 * @brief Opens an input file and redirects standard input to it.
 *
 * This function opens the specified input file for reading and
 * redirects the standard input (stdin) to read from this file.
 * If the file cannot be opened, it prints an error message and
 * exits the process.
 *
 * @param input_file The name of the input file to open.
 */
void	open_and_redirect_input(char *input_file)
{
	int	fd_in;

	fd_in = open(input_file, O_RDONLY);
	if (fd_in == -1)
	{
		ft_error_and_exit(input_file, strerror(errno), EXIT_FAILURE);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	}
	close(fd_in);
}

/**
 * @brief Opens an output file and redirects standard output to it.
 *
 * This function opens the specified output file for writing, either
 * in append mode or truncation mode based on the `output_mode` parameter.
 * It then redirects the standard output (stdout) to write to this file.
 * If the file cannot be opened, it prints an error message and exits
 * the process.
 *
 * @param output_file The name of the output file to open.
 * @param output_mode The mode for opening the file:
 *                    1 for truncation, 2 for append.
 */
void	open_and_redirect_output(char *output_file, int output_mode)
{
	int	fd_out;
	int	flags;

	if (output_mode == 2)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd_out = open(output_file, flags, 0644);
	if (fd_out == -1)
	{
		ft_error_and_exit(output_file, strerror(errno), EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	}
	close(fd_out);
}

/**
 * @brief Checks if a token is a redirection operator.
 *
 * This function checks if the given string matches any of the
 * standard redirection operators: ">", "<", ">>", or "<<".
 *
 * @param token The string to check.
 * @return 1 if the string is a redirection operator, 0 otherwise.
 */
int	is_redirection(const char *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token, "<", 2) == 0)
		return (1);
	if (ft_strncmp(token, ">", 2) == 0)
		return (1);
	if (ft_strncmp(token, ">>", 3) == 0)
		return (1);
	if (ft_strncmp(token, "<<", 3) == 0)
		return (1);
	return (0);
}
