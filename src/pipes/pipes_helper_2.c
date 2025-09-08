/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helper_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:01:47 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 09:10:04 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a single command in a child process.
 *
 * This function checks if the command is a builtin and runs it. If not, it
 * executes an external command. The function exits the child process after
 * completion, handling any execution errors.
 *
 * @param cmd The command to execute.
 * @param data A pointer to the process data structure.
 */
void	execute_child_command(t_command *cmd, t_process_data *data)
{
	int	status;

	if (cmd->args == NULL || cmd->args[0] == NULL)
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		status = run_builtin(cmd->args, data);
		cleanup_child_pipeline_resources(data);
		exit(status);
	}
	execute_cmd(cmd->cmd_path, cmd->args,
		data->path_dirs, data->env_list);
	ft_error_and_exit("execve", strerror(errno), EXIT_FAILURE);
}

/**
 * @brief Processes the exit status of a single child process.
 *
 * This function checks if a child process exited normally or was terminated
 * by a signal, updating the `last_status` and `signal_printed` flag
 * in the provided struct.
 *
 * @param status The exit status of the child process.
 * @param info A pointer to the `t_wait_info` struct.
 * @param is_last_child A flag indicating if this is the last child in the list.
 */
static void	process_child_status(int status, t_wait_info *info,
										int is_last_child)
{
	int	sig;

	if (WIFEXITED(status))
	{
		if (is_last_child)
			info->last_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (!(info->signal_printed))
		{
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			info->signal_printed = 1;
		}
		if (is_last_child)
			info->last_status = 128 + sig;
	}
}

/**
 * @brief Waits for all child processes to complete.
 *
 * This function recursively waits for each process in the `pids` array,
 * from the starting `index` to the `max` count. It processes each child's
 * exit status to determine the final return value.
 *
 * @param pids An array of process IDs.
 * @param index The current index in the `pids` array.
 * @param max The total number of processes to wait for.
 * @param info A pointer to the `t_wait_info` struct.
 * @return The final exit status of the last command in the pipeline.
 */
static int	wait_all_children(pid_t *pids, int index, int max,
									t_wait_info *info)
{
	int	status;
	int	is_last_child;

	if (index >= max)
		return (info->last_status);
	if (waitpid(pids[index], &status, 0) == -1)
	{
		ft_error_and_exit("waitpid", strerror(errno), EXIT_FAILURE);
		return (wait_all_children(pids, index + 1, max, info));
	}
	is_last_child = (index == max - 1);
	process_child_status(status, info, is_last_child);
	return (wait_all_children(pids, index + 1, max, info));
}

/**
 * @brief Executes the core logic of a command pipeline.
 *
 * This function handles closing pipes, waiting for all child processes,
 * and freeing allocated resources. It is called by `run_command_pipeline`.
 *
 * @param data The `t_process_data` struct containing pipeline information.
 * @param cmd_count The number of commands in the pipeline.
 * @return The exit status of the last command in the pipeline.
 */
int	run_pipeline_core(t_process_data *data, int cmd_count)
{
	int			exit_status;
	t_wait_info	wait_state;

	wait_state.last_status = 0;
	wait_state.signal_printed = 0;
	close_free_pipes_recursively(data->pipes, 0, cmd_count - 1);
	exit_status = wait_all_children(data->pids, 0, cmd_count, &wait_state);
	data->last_exit_status = exit_status;
	free(data->pids);
	return (exit_status);
}
