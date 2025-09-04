/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helper_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:20:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 10:08:11 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Sets up I/O for a child process in a pipeline.
 *
 * This function handles pipe duplication and redirection, ensuring the child's
 * stdin and stdout are correctly wired to the pipeline. It also closes all
 * unnecessary pipe file descriptors to prevent resource leaks.
 *
 * @param data A pointer to the process data structure.
 * @param i The index of the current command in the pipeline.
 */
static void	setup_child_io(t_process_data *data, int i)
{
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
	close_free_pipes_recursively(data->pipes, 0, data->cmd_count - 1);
	redirect_io(data->cmds[i]->input_file, data->cmds[i]->output_file,
		data->cmds[i]->output_mode);
}

/**
 * @brief Recursively forks processes for a command pipeline.
 *
 * This function iterates through a list of commands, forking
 * a new process for each and setting up their I/O connections
 * to form a pipeline. It manages signal handlers for the child
 * processes and delegates command execution to a helper function.
 *
 * @param data A pointer to the process data structure.
 * @param i The current index of the command to process.
 * @return 0 on success, -1 on failure.
 */
int	fork_all_processes_recursive(t_process_data *data, int i)
{
	if (i >= data->cmd_count)
		return (0);
	data->pids[i] = fork();
	if (data->pids[i] == -1)
	{
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (data->pids[i] == 0)
	{
		reset_child_signal_handlers();
		setup_child_io(data, i);
		execute_child_command(data->cmds[i], data);
	}
	return (fork_all_processes_recursive(data, i + 1));
}

/**
 * @brief Creates an array to hold child process IDs.
 *
 * This function allocates memory for an array of `pid_t` to store
 * the process IDs of child processes created for each command in
 * the pipeline. If allocation fails, it cleans up any allocated
 * pipe file descriptors.
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
		close_free_pipes_recursively(pipes, 0, cmd_count - 1);
		return (NULL);
	}
	return (child_processes);
}

/**
 * @brief Creates pipe file descriptors for inter-process communication.
 *
 * This function allocates memory for an array of pipes and initializes
 * each pipe using the `pipe` system call. It uses recursion to create
 * each pipe from the current index to the maximum number of pipes.
 * If any allocation or pipe creation fails, it cleans up by freeing
 * already created pipes.
 * @param cmd_count  The number of commands in the pipeline.
 * @return A pointer to a 2D array of pipe file descriptors on success,
 *         or NULL if allocation or pipe creation fails.
 */
int	create_all_pipes_recursively(int **pipes, int index, int max)
{
	if (index >= max)
		return (0);
	pipes[index] = malloc(sizeof(int) * 2);
	if (pipes[index] == NULL)
		return (-1);
	if (pipe(pipes[index]) == -1)
	{
		ft_error_and_exit("pipe", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	if (create_all_pipes_recursively(pipes, index + 1, max) == -1)
	{
		free(pipes[index]);
		return (-1);
	}
	return (0);
}

/**
 * @brief Recursively closes and frees all pipe file descriptors.
 *
 * This function iterates through the array of pipes, closing both
 * ends of each pipe and freeing the allocated memory. It uses recursion
 * to process each pipe from the current index to the maximum.
 * @param pipes  A 2D array of pipe file descriptors.
 * @param idx    The current index in the pipes array.
 * @param max    The total number of pipes.
 */
void	close_free_pipes_recursively(int **pipes, int idx, int max)
{
	if (pipes == NULL || idx >= max)
		return ;
	close(pipes[idx][0]);
	close(pipes[idx][1]);
	free(pipes[idx]);
	close_free_pipes_recursively(pipes, idx + 1, max);
	if (idx == 0)
		free(pipes);
}
