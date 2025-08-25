/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:27:49 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/25 10:01:18 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Redirects input and output for built-in commands.
 *
 * This function handles redirection of input and output for built-in commands
 * like `cd`, `pwd`, etc. It opens the files and uses dup2 to redirect
 * STDIN and/or STDOUT.
 * @param input_file The file to redirect STDIN from.
 * @param output_file The file to redirect STDOUT to.
 * @return 0 on success, -1 on failure.
 */
static int	redirect_builtin_input(char *input_file)
{
	int	fd;

	if (!input_file)
		return (0);
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_error_and_exit(input_file, strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/**
 * @brief Redirects input and output for built-in commands.
 *
 * This function handles redirection of input and output for built-in commands
 * like `cd`, `pwd`, etc. It opens the files and uses dup2 to redirect
 * STDIN and/or STDOUT.
 * @param input_file The file to redirect STDIN from.
 * @param output_file The file to redirect STDOUT to.
 * @return 0 on success, -1 on failure.
 */
static int	redirect_builtin_output(char *output_file, int output_mode)
{
	int	fd;
	int	flags;

	if (!output_file)
		return (0);
	if (output_mode == 2)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(output_file, flags, 0644);
	if (fd == -1)
	{
		ft_error_and_exit(output_file, strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/**
 * @brief Safely applies I/O redirection for built-in commands.
 *
 * This function opens the necessary files and uses dup2 to redirect
 * STDIN and/or STDOUT. It returns -1 on failure instead of exiting.
 * @param input_file The file to redirect STDIN from.
 * @param output_file The file to redirect STDOUT to.
 * @return 0 on success, -1 on failure.
 */
int	apply_builtin_redirection(char *input_file, char *output_file,
														int output_mode)
{
	if (redirect_builtin_input(input_file) == -1)
		return (-1);
	if (redirect_builtin_output(output_file, output_mode) == -1)
		return (-1);
	return (0);
}
