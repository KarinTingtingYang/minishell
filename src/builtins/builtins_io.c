/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:27:49 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 11:29:30 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Redirects input for built-in commands.
 *
 * This function handles redirection of input for built-in commands
 * like `cd`, `pwd`, etc. It opens the file and uses dup2 to redirect
 * STDIN.
 * @param input_file The file to redirect STDIN from.
 * @return 0 on success, -1 on failure.
 */
static int	redirect_builtin_input(char *input_file)
{
	int	fd;

	if (input_file == NULL)
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
 * @brief Redirects output for built-in commands.
 *
 * This function handles redirection of output for built-in commands
 * like `cd`, `pwd`, etc. It opens the file and uses dup2 to redirect
 * STDOUT.
 * @param output_file The file to redirect STDOUT to.
 * @param output_mode 1 for overwrite (>) and 2 for append (>>).
 * @return 0 on success, -1 on failure.
 */
static int	redirect_builtin_output(char *output_file, int output_mode)
{
	int	fd;
	int	flags;

	if (output_file == NULL)
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
 * @brief Applies input and output redirections for built-in commands.
 *
 * This function combines input and output redirection handling for
 * built-in commands. It calls the respective functions to set up
 * the redirections.
 * @param input_file The file to redirect STDIN from (NULL if no redirection).
 * @param output_file The file to redirect STDOUT to (NULL if no redirection).
 * @param output_mode 1 for overwrite (>) and 2 for append (>>).
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
