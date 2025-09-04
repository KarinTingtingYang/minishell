/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_prepared_command.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 16:54:16 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 10:36:27 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the execution of a builtin command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the builtin command.
 */
static int	execute_builtin(t_execute_data *data, t_process_data *process_data)
{
	run_builtin(data->clean_args, process_data);
	free_execute_data(data);
	return (0);
}

/**
 * @brief Executes the child process logic.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 */
static void	execute_child_process(t_execute_data *data,
	t_process_data *process_data)
{
	redirect_io(data->input_file, data->output_file, data->output_mode);
	if (is_builtin(data->clean_args[0]))
		run_builtin(data->clean_args, process_data);
	else
		execute_cmd(data->cmd_path, data->clean_args, data->path_dirs,
			data->env_list);
	exit(0);
}

/**
 * @brief Handles the parent process logic after forking.
 *
 * @param pid The PID of the child process.
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
static int	handle_parent_process(pid_t pid, t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	exit_code = wait_for_child_and_handle_status(pid);
	setup_signal_handlers();
	process_data->last_exit_status = exit_code;
	free_execute_data(data);
	return (exit_code);
}

/**
 * @brief Forks the process and executes the command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
static int	fork_and_execute(t_execute_data *data, t_process_data *process_data)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
		free_execute_data(data);
		return (-1);
	}
	else if (pid == 0)
		execute_child_process(data, process_data);
	else
		return (handle_parent_process(pid, data, process_data));
	return (0);
}

/**
 * @brief Executes a previously prepared command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
int	execute_prepared_command(t_execute_data *data, t_process_data *process_data)
{
	int	has_redirection;

	has_redirection = (data->input_file || data->output_file);
	if (is_builtin(data->clean_args[0])
		&& !has_redirection
		&& !process_data->in_pipeline)
		return (execute_builtin(data, process_data));
	return (fork_and_execute(data, process_data));
}
