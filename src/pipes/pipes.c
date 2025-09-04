/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:08:55 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 10:05:22 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Prepares pipe file descriptors for a command pipeline.
 *
 * This function allocates and initializes the necessary pipe file descriptors
 * for inter-process communication between commands in a pipeline. It handles
 * memory allocation and error checking, returning NULL if any step fails.
 *
 * @param cmd_count The number of commands in the pipeline.
 * @return A pointer to a 2D array of pipe file descriptors, or NULL on failure.
 */
static int	**create_pipe_fds_between_commands(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count < 2)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (pipes == NULL)
		return (NULL);
	if (create_all_pipes_recursively(pipes, 0, cmd_count - 1) == -1)
	{
		i = 0;
		while (i < cmd_count - 1)
		{
			if (pipes[i] != NULL)
				free(pipes[i]);
			i++;
		}
		free(pipes);
		return (NULL);
	}
	return (pipes);
}

/**
 * @brief Prepares pipe file descriptors for a command pipeline.
 *
 * This function creates the necessary pipes for inter-process
 * communication between commands in a pipeline. It allocates
 * memory for the pipe file descriptors and initializes them.
 * If allocation or pipe creation fails, it returns NULL.
 * @param cmd_count The number of commands in the pipeline.
 * @return A pointer to a 2D array of pipe file descriptors on success,
 *         or NULL if allocation or pipe creation fails.
 */
static int	**prepare_pipe_fds(int cmd_count)
{
	int	**pipes;

	pipes = create_pipe_fds_between_commands(cmd_count);
	if (pipes == NULL)
		return (NULL);
	return (pipes);
}

/**
 * @brief Sets up and forks all processes for a command pipeline.
 *
 * This function initializes the necessary data structures for
 * a command pipeline, including pipes and process ID storage,
 * and then recursively forks all child processes.
 * It returns an integer indicating success or failure.
 *
 * @param data A pointer to the process data structure containing
 *             all information.
 * @return 1 on success, 0 if no command is to be run, or -1 on a fatal error.
 */
static int	setup_and_fork_pipeline(t_process_data *data)
{
	if (data->cmd_count < 1)
		return (0);
	data->pipes = NULL;
	data->pids = NULL;
	if (data->cmd_count > 1)
	{
		data->pipes = prepare_pipe_fds(data->cmd_count);
		if (data->pipes == NULL)
			return (-1);
	}
	data->pids = create_child_processes(data->cmd_count, data->pipes);
	if (data->pids == NULL)
	{
		return (-1);
	}
	if (fork_all_processes_recursive(data, 0) == -1)
		return (-1);
	return (1);
}

/**
 * @brief Sets up and executes the command pipeline.
 *
 * This function calls the `setup_and_fork_pipeline` function to create the
 * processes for the pipeline. It now only requires the data struct, as all
 * other arguments are stored within it.
 *
 * @param data A pointer to the `t_process_data` struct.
 * @return The fork status from the pipeline setup.
 */
static int	setup_and_execute_pipeline(t_process_data *data)
{
	return (setup_and_fork_pipeline(data));
}

/**
 * @brief Runs a series of commands connected by pipes.
 *
 * This function sets up the necessary pipes and forks child processes
 * for each command in the pipeline. It manages signal handling and
 * waits for all child processes to complete, returning the exit status
 * of the last command executed.
 *
 * @param cmds An array of command structures to execute.
 * @param cmd_count The number of commands in the array.
 * @param path_dirs An array of directories to search for executables.
 * @param env_list A linked list of environment variables.
 * @return The exit status of the last command in the pipeline,
 *         or -1 on failure.
 */
int	run_command_pipeline(t_command **cmds, int cmd_count, char **path_dirs,
							t_env_var *env_list)
{
	t_process_data	data;
	int				fork_status;
	int				exit_status;

	data.cmds = cmds;
	data.cmd_count = cmd_count;
	data.path_dirs = path_dirs;
	data.env_list = env_list;
	data.last_exit_status = 0;
	data.in_pipeline = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	fork_status = setup_and_execute_pipeline(&data);
	if (fork_status == -1)
	{
		free(data.pids);
		close_free_pipes_recursively(data.pipes, 0, cmd_count - 1);
		setup_signal_handlers();
		return (-1);
	}
	exit_status = run_pipeline_core(&data, cmd_count);
	setup_signal_handlers();
	return (exit_status);
}
