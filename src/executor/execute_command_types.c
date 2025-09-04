/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_command_types.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 19:24:59 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 10:34:55 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a builtin command with proper redirection handling.
 *
 * This function saves the original stdin and stdout file descriptors,
 * applies the necessary redirections for the builtin command, executes
 * the command, and then restores the original file descriptors.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @return The exit status of the executed builtin command.
 */
int	execute_builtin_command(t_execute_data *data, t_process_data *process_data)
{
	int	original_stdin;
	int	original_stdout;
	int	exit_status;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (apply_builtin_redirection(data->input_file,
			data->output_file, data->output_mode) == -1)
		exit_status = 1;
	else
		exit_status = run_builtin(data->clean_args, process_data);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (exit_status);
}

/**
 * @brief Executes an external command by finding its full path and running it.
 *
 * This function retrieves the PATH directories 
 * from the environment variable list,
 * finds the full path of the command, and then executes it using the prepared
 * command execution function.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @param env_list The environment variable list.
 * @return The exit status of the executed external command.
 */
int	execute_external_command(t_execute_data *data, t_process_data *process_data,
	t_env_var *env_list)
{
	char	**path_dirs;
	int		exit_status;

	path_dirs = find_path_dirs(env_list);
	data->cmd_path = find_full_cmd_path(data->clean_args[0], path_dirs);
	free_split(path_dirs);
	exit_status = execute_prepared_command(data, process_data);
	return (exit_status);
}

/**
 * @brief Handles cases where only redirection is specified without a command.
 *
 * This function checks if the input file can be opened for reading and
 * sets the last exit status accordingly. It frees the execute data structure
 * before returning.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @return 0 if successful, or 1 if there was an error opening the input file.
 */
int	handle_redirection_only(t_execute_data *data, t_process_data *process_data)
{
	int	exit_status;
	int	fd;

	exit_status = 0;
	if (data->input_file)
	{
		fd = open(data->input_file, O_RDONLY);
		if (fd == -1)
		{
			ft_error(data->input_file, strerror(errno));
			exit_status = 1;
		}
		else
		{
			close(fd);
		}
	}
	process_data->last_exit_status = exit_status;
	free_execute_data(data);
	return (0);
}

/**
 * @brief Executes a single command, handling builtins, externals, 
 * and redirections.
 *
 * This function determines if the command is a builtin or an external command,
 * handles any input/output redirections, and executes the command accordingly.
 * It also manages error handling for redirection issues and updates the
 * process data with the last exit status.
 *
 * @param args The tokenized command arguments.
 * @param env_list The environment variable list.
 * @param process_data Pointer to the global process data structure.
 * @return The exit status of the executed command.
 */
int	execute_single_command(char **args, t_env_var *env_list,
	t_process_data *process_data)
{
	t_execute_data	data;
	int				exit_status;

	ft_bzero(&data, sizeof(t_execute_data));
	data.env_list = env_list;
	process_data->in_pipeline = 0;
	data.clean_args = handle_redirection(args, process_data, &data);
	if (!data.clean_args)
		return (handle_redirection_error(&data, process_data, args));
	if (!data.clean_args[0])
		return (handle_redirection_only(&data, process_data));
	if (is_builtin(data.clean_args[0]))
		exit_status = execute_builtin_command(&data, process_data);
	else
		exit_status = execute_external_command(&data, process_data, env_list);
	process_data->last_exit_status = exit_status;
	free_execute_data(&data);
	return (exit_status);
}
