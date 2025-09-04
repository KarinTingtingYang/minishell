/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor_error.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 19:07:14 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 10:24:11 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles errors related to stat() system call.
 *
 * This function checks the error code from a failed stat() call and
 * prints an appropriate error message before exiting with a specific
 * exit code.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed stat() call.
 */
void	handle_stat_error(char **envp, char **args, int error_code)
{
	if (envp)
		free_split(envp);
	if (error_code == ENOTDIR)
		ft_error_and_exit(args[0], "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit(args[0], "No such file or directory", 127);
	ft_error_and_exit(args[0], strerror(error_code), 126);
}

/**
 * @brief Handles errors related to access() system call.
 *
 * This function checks the error code from a failed access() call and
 * prints an appropriate error message before exiting with a specific
 * exit code.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed access() call.
 */
void	handle_access_error(char **envp, char **args, int error_code)
{
	if (envp)
		free_split(envp);
	if (error_code == EACCES)
		ft_error_and_exit(args[0], "Permission denied", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit(args[0], "Not a directory", 126);
	ft_error_and_exit(args[0], strerror(error_code), 126);
}

/**
 * @brief Handles errors related to execve() system call.
 *
 * This function checks the error code from a failed execve() call and
 * prints an appropriate error message before exiting with a specific
 * exit code.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed execve() call.
 */
void	handle_execve_error(char **envp, char **args, int error_code)
{
	free_split(envp);
	if (error_code == ENOEXEC)
		ft_error_and_exit(args[0], "Exec format error", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit(args[0], "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit(args[0], "No such file or directory", 127);
	else if (error_code == EACCES)
		ft_error_and_exit(args[0], "Permission denied", 126);
	ft_error_and_exit(args[0], strerror(error_code), 126);
}

/**
 * @brief Handles the case when the command path is NULL.
 *
 * This function checks if the command contains a '/' character to determine
 * if it was specified with a path. It then prints an appropriate error message
 * and exits with a specific exit code.
 *
 * @param args The command arguments array.
 * @param envp The environment variables array to free if needed.
 */
void	handle_null_cmd_path(char **args, char **envp)
{
	if (args && args[0] && ft_strchr(args[0], '/'))
	{
		if (envp)
			free_split(envp);
		ft_error_and_exit(args[0], "No such file or directory", 127);
	}
	if (envp)
		free_split(envp);
	if (args && args[0])
		ft_error_and_exit(args[0], "command not found", 127);
	else
		ft_error_and_exit("minishell", "command not found", 127);
}

/**
 * @brief Handles errors related to command redirection syntax.
 *
 * This function checks for signal interrupts and syntax errors in the
 * command arguments related to redirection. It sets the last exit status
 * in the process data structure accordingly.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @param args The command arguments array.
 * @return The exit status code based on the error encountered.
 */
int	handle_redirection_error(t_execute_data *data, t_process_data *process_data,
	char **args)
{
	int	last_arg_index;

	(void)data;
	if (g_signal_received == SIGINT)
	{
		process_data->last_exit_status = 130;
		return (130);
	}
	last_arg_index = 0;
	while (args[last_arg_index + 1] != NULL)
		last_arg_index++;
	if (is_redirection(args[last_arg_index]))
	{
		process_data->last_exit_status = 2;
		return (2);
	}
	process_data->last_exit_status = 1;
	return (1);
}
