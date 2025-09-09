/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helper_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:20:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:23:38 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../../libft/get_next_line/get_next_line.h"
#include <errno.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*                         Child I/O and pipe handling                        */
/* -------------------------------------------------------------------------- */

/**
 * @brief Sets up I/O for a child process in a pipeline.
 *
 * Duplicates the correct pipe ends to STDIN/STDOUT, then closes and frees
 * all pipe FDs to avoid descriptor leaks in the child. Finally applies any
 * command-level redirections.
 *
 * @param data A pointer to the process data structure.
 * @param i The index of the current command in the pipeline.
 */
static void	setup_child_io(t_process_data *data, int i)
{
	int	max_pipes;

	max_pipes = data->cmd_count - 1;
	if (i > 0)
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) == -1)
			ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	}
	if (i < data->cmd_count - 1)
	{
		if (dup2(data->pipes[i][1], STDOUT_FILENO) == -1)
			ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	}
	close_free_pipes_recursively(data->pipes, 0, max_pipes);
	redirect_io(data->cmds[i]->input_file, data->cmds[i]->output_file,
		data->cmds[i]->output_mode);
}

/* -------------------------------------------------------------------------- */
/*                              Process forking                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Recursively forks processes for a command pipeline.
 *
 * For each command, fork a child, reset child signals, set up I/O, and
 * execute the command. On fork error, closes all pipes and frees parent
 * resources before exiting to avoid "still reachable" leaks.
 *
 * @param data A pointer to the process data structure.
 * @param i The current index of the command to process.
 * @return 0 on success, -1 on failure (unreachable if we exit on error).
 */
int	fork_all_processes_recursive(t_process_data *data, int i)
{
	if (i >= data->cmd_count)
		return (0);
	data->pids[i] = fork();
	if (data->pids[i] == -1)
	{
		close_free_pipes_recursively(data->pipes, 0, data->cmd_count - 1);
		cleanup_pipeline_resources(data);
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (data->pids[i] == 0)
	{
		get_next_line_cleanup();
		reset_child_signal_handlers();
		setup_child_io(data, i);
		execute_child_command(data->cmds[i], data);
		if (data->last_exit_status != 0)
			exit(data->last_exit_status);
		else
			exit(127);
	}
	return (fork_all_processes_recursive(data, i + 1));
}

/* -------------------------------------------------------------------------- */
/*                              PID allocation                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Creates an array to hold child process IDs.
 *
 * On allocation failure, closes and frees all existing pipes to avoid leaks.
 *
 * @param cmd_count The number of commands in the pipeline.
 * @param pipes     A 2D array of pipe file descriptors to clean up on failure.
 * @return A pointer to an array of `pid_t` on success, or NULL on failure.
 */
pid_t	*create_child_processes(int cmd_count, int **pipes)
{
	pid_t	*child_processes;

	child_processes = malloc(sizeof(pid_t) * cmd_count);
	if (child_processes == NULL)
	{
		if (pipes)
			close_free_pipes_recursively(pipes, 0, cmd_count - 1);
		return (NULL);
	}
	return (child_processes);
}

/* -------------------------------------------------------------------------- */
/*                          Pipe creation and cleanup                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Recursively create @p max pipes: indices [0, max).
 *
 * On error at any level:
 *  - If malloc fails for the current @p index, return -1 (caller should clean).
 *  - If pipe() fails, free the just-allocated slot, close & free all
 *    previously created pipes [0, index), then exit with an error AFTER
 *    cleanup to avoid "still reachable" reports.
 *
 * @param pipes  Preallocated array of int[2] pointers of length @p max.
 * @param index  Current pipe index being created.
 * @param max    Total number of pipes to create (usually cmd_count - 1).
 * @return 0 on success, -1 on (recoverable) allocation error.
 */
int	create_all_pipes_recursively(int **pipes, int index, int max)
{
	if (index >= max)
		return (0);
	pipes[index] = (int *)malloc(sizeof(int) * 2);
	if (pipes[index] == NULL)
		return (-1);
	if (pipe(pipes[index]) == -1)
	{
		free(pipes[index]);
		pipes[index] = NULL;
		close_free_pipes_recursively(pipes, 0, index);
		ft_error_and_exit("pipe", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (create_all_pipes_recursively(pipes, index + 1, max) == -1)
	{
		close(pipes[index][0]);
		close(pipes[index][1]);
		free(pipes[index]);
		pipes[index] = NULL;
		return (-1);
	}
	return (0);
}

/**
 * @brief Recursively closes and frees all pipe file descriptors.
 *
 * Closes both ends of each pipe at indices [idx, max) and frees the
 * per-pipe buffers. On the final unwind (idx == 0) also frees the
 * top-level @p pipes array pointer.
 *
 * @param pipes  A 2D array of pipe file descriptors.
 * @param idx    The current index in the pipes array.
 * @param max    The total number of pipes (exclusive upper bound).
 */
void	close_free_pipes_recursively(int **pipes, int idx, int max)
{
	if (pipes == NULL || idx >= max)
		return ;
	if (pipes[idx])
	{
		close(pipes[idx][0]);
		close(pipes[idx][1]);
		free(pipes[idx]);
		pipes[idx] = NULL;
	}
	close_free_pipes_recursively(pipes, idx + 1, max);
	if (idx == 0)
	{
		free(pipes);
	}
}
