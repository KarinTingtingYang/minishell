/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes_helper_3.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 13:00:11 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/09 13:05:14 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the execution of a builtin command in a child process.
 *
 * This function runs the specified builtin command and ensures that
 * all child-owned pipeline resources are cleaned up before exiting
 * with the command's exit status.
 *
 * @param cmd The command structure containing the builtin command details.
 * @param data Pointer to the process data structure for resource management.
 */
void	handle_builtin_command(t_command *cmd, t_process_data *data)
{
	int	status;

	status = run_builtin(cmd->args, data);
	cleanup_child_pipeline_resources(data);
	exit(status);
}

/**
 * @brief Handles errors when command path resolution fails.
 *
 * This function checks if the command name contains a slash to determine
 * if it was meant to be a path. It frees the environment and cleans up
 * resources before exiting with an appropriate error message and code.
 *
 * @param cmd_name The name of the command that failed to resolve.
 * @param envp The environment variables array.
 * @param data Pointer to the process data structure for cleanup.
 */
void	handle_path_resolution_error(char *cmd_name, char **envp,
									t_process_data *data)
{
	int	has_slash;

	has_slash = (ft_strchr(cmd_name, '/') != NULL);
	free_split(envp);
	cleanup_child_pipeline_resources(data);
	if (has_slash)
		ft_error_and_exit(cmd_name, "No such file or directory", 127);
	ft_error_and_exit(cmd_name, "command not found", 127);
}

/**
 * @brief Handles errors from stat() calls in the child process.
 *
 * This function frees the environment and cleans up resources before
 * exiting with an appropriate error message based on the error code.
 *
 * @param cmd_name The name of the command that caused the error.
 * @param envp The environment variables array.
 * @param data Pointer to the process data structure for cleanup.
 * @param error_code The errno value from the failed stat() call.
 */
void	handle_stat_error_child(char *cmd_name, char **envp,
						t_process_data *data, int error_code)
{
	free_split(envp);
	cleanup_child_pipeline_resources(data);
	if (error_code == ENOTDIR)
		ft_error_and_exit(cmd_name, "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit(cmd_name, "No such file or directory", 127);
	ft_error_and_exit(cmd_name, strerror(error_code), 126);
}

/**
 * @brief Handles errors from access() calls in the child process.
 *
 * This function frees the environment and cleans up resources before
 * exiting with an appropriate error message based on the error code.
 *
 * @param cmd_name The name of the command that caused the error.
 * @param envp The environment variables array.
 * @param data Pointer to the process data structure for cleanup.
 * @param error_code The errno value from the failed access() call.
 */
void	handle_access_error_child(char *cmd_name, char **envp,
						t_process_data *data, int error_code)
{
	free_split(envp);
	cleanup_child_pipeline_resources(data);
	if (error_code == EACCES)
		ft_error_and_exit(cmd_name, "Permission denied", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit(cmd_name, "Not a directory", 126);
	ft_error_and_exit(cmd_name, strerror(error_code), 126);
}

/**
 * @brief Handles errors from execve() calls in the child process.
 *
 * This function frees the environment and cleans up resources before
 * exiting with an appropriate error message based on the error code.
 *
 * @param cmd_name The name of the command that caused the error.
 * @param envp The environment variables array.
 * @param data Pointer to the process data structure for cleanup.
 * @param error_code The errno value from the failed execve() call.
 */
void	handle_execve_error_child(char *cmd_name, char **envp,
						t_process_data *data, int error_code)
{
	free_split(envp);
	cleanup_child_pipeline_resources(data);
	if (error_code == ENOEXEC)
		ft_error_and_exit(cmd_name, "Exec format error", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit(cmd_name, "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit(cmd_name, "No such file or directory", 127);
	else if (error_code == EACCES)
		ft_error_and_exit(cmd_name, "Permission denied", 126);
	ft_error_and_exit(cmd_name, strerror(error_code), 126);
}
