/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prepared_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:54:16 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/09 12:43:15 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../../libft/get_next_line/get_next_line.h"
#include <signal.h>
#include <errno.h>

/**
 * @brief Free all dynamically allocated members of t_execute_data.
 *
 * This function is responsible for freeing all the dynamically allocated
 * memory used by the t_execute_data structure. It should be called
 * when the execution of a command is complete and the data is no longer needed.
 *
 * @param data Pointer to the execution data to free.
 * @return error code (0 on success, -1 on failure).
 */
static int	execute_builtin_in_parent(t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	exit_code = run_builtin(data->clean_args, process_data);
	process_data->last_exit_status = exit_code;
	free_execute_data(data);
	return (exit_code);
}

/**
 * @brief Executes the command in the child process.
 * This function sets up the child process environment,
 * handles built-in commands, and executes external commands.
 * It ensures proper cleanup of resources before exiting.
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return void (exits the child process).
 */
static void	execute_child_process(t_execute_data *data,
										t_process_data *process_data)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	get_next_line_cleanup();
	redirect_io(data->input_file, data->output_file,
		data->output_mode);
	if (is_builtin(data->clean_args[0]))
	{
		exit_code = run_builtin(data->clean_args, process_data);
		free_execute_data(data);
		exit(exit_code);
	}
	execute_cmd(data->cmd_path, data->clean_args,
		data->path_dirs, data->env_list);
	if (process_data->last_exit_status != 0)
	{
		exit_code = process_data->last_exit_status;
	}
	else
	{
		exit_code = 127;
	}
	free_execute_data(data);
	exit(exit_code);
}

/**
 * @brief Handles the parent process after forking.
 * Waits for the child process to finish, retrieves its exit status,
 * frees execution data, restores signal handlers, and updates the
 * last exit status in the process data.
 * @param pid The PID of the child process.
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the child process.
 * */
static int	handle_parent_process(pid_t pid, t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	exit_code = wait_for_child_and_handle_status(pid);
	free_execute_data(data);
	setup_signal_handlers();
	process_data->last_exit_status = exit_code;
	return (exit_code);
}

/**
 * @brief Forks a new process to execute the command.
 * Handles errors during forking and ensures proper cleanup.
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command, or -1 on fork failure.
 */
static int	fork_and_execute(t_execute_data *data, t_process_data *process_data)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		free_execute_data(data);
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	else if (pid == 0)
	{
		execute_child_process(data, process_data);
	}
	return (handle_parent_process(pid, data, process_data));
}

/**
 * @brief Executes a prepared command, handling built-ins in the parent
 *        when possible, or forking for external commands or redirections.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command, or -1 on fork failure.
 */
int	execute_prepared_command(t_execute_data *data, t_process_data *process_data)
{
	int	has_redirection;

	has_redirection = (data->input_file || data->output_file);
	if (is_builtin(data->clean_args[0]) && !has_redirection
		&& !process_data->in_pipeline)
		return (execute_builtin_in_parent(data, process_data));
	return (fork_and_execute(data, process_data));
}
