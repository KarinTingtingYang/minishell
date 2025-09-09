/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:08:55 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 09:34:19 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <signal.h>

/* -------------------------------------------------------------------------- */
/*                           Pipe allocation helpers                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Prepares pipe file descriptors for a command pipeline.
 *
 * Allocates (cmd_count - 1) pipes and initializes each with pipe().
 * On failure, closes any already-created FDs and frees the array to
 * avoid descriptor and heap leaks.
 *
 * @param cmd_count The number of commands in the pipeline.
 * @return int** pointer to allocated pipes array, or NULL on failure.
 */
static int	**create_pipe_fds_between_commands(int cmd_count)
{
	int	**pipes;

	if (cmd_count < 2)
		return (NULL);
	pipes = (int **)malloc(sizeof(int *) * (cmd_count - 1));
	if (pipes == NULL)
		return (NULL);
	/* Initialize to NULL so cleanup can safely iterate all slots */
	for (int i = 0; i < cmd_count - 1; ++i)
		pipes[i] = NULL;
	if (create_all_pipes_recursively(pipes, 0, cmd_count - 1) == -1)
	{
		/* Close any pipes that were created and free top-level array */
		close_free_pipes_recursively(pipes, 0, cmd_count - 1);
		return (NULL);
	}
	return (pipes);
}

/**
 * @brief Thin wrapper to build pipe FDs array.
 */
static int	**prepare_pipe_fds(int cmd_count)
{
	return (create_pipe_fds_between_commands(cmd_count));
}

/* -------------------------------------------------------------------------- */
/*                            Forking / pipeline run                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Sets up and forks all processes for a command pipeline.
 *
 * Ensures that on any failure path we do not leave allocated memory or
 * open FDs behind (caller also does best-effort cleanup).
 *
 * @param data Process data containing commands and counts.
 * @return 1 on success, 0 if nothing to run, -1 on fatal error.
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
		/* Pipes may have been created; caller will close them too */
		return (-1);
	}
	if (fork_all_processes_recursive(data, 0) == -1)
		return (-1);
	return (1);
}

/**
 * @brief Thin wrapper over setup_and_fork_pipeline.
 */
static int	setup_and_execute_pipeline(t_process_data *data)
{
	return (setup_and_fork_pipeline(data));
}

/* -------------------------------------------------------------------------- */
/*                                Public API                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief Runs a series of commands connected by pipes.
 *
 * Sets SIGINT/SIGQUIT to ignore in the parent during forking, then restores
 * handlers afterward. On any failure, frees pids and closes all pipes to
 * avoid "still reachable" reports.
 *
 * @param data Process data with cmds/cmd_count/path_dirs/env_list filled in.
 * @return Exit status of the last command, or -1 on failure.
 */
int	run_command_pipeline(t_process_data *data)
{
	int	fork_status;
	int	exit_status;

	data->last_exit_status = 0;
	data->in_pipeline = 1;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	fork_status = setup_and_execute_pipeline(data);
	if (fork_status == -1)
	{
		/* Best-effort cleanup on failure */
		free(data->pids);
		data->pids = NULL;
		close_free_pipes_recursively(data->pipes, 0, data->cmd_count - 1);
		data->pipes = NULL;

		setup_signal_handlers();
		return (-1);
	}
	/* Success: parent closes pipes & waits within run_pipeline_core */
	exit_status = run_pipeline_core(data, data->cmd_count);

	setup_signal_handlers();
	return (exit_status);
}
