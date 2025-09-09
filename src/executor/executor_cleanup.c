/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:58:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 09:35:51 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../../libft/get_next_line/get_next_line.h"

/* -------------------------------------------------------------------------- */
/*                              Local utilities                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Free a single pointer and set it to NULL.
 *        Accepts address of the pointer.
 */
static void	free_ptr(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
	}
}

/**
 * @brief Iteratively free an array of t_command* of length @p count.
 *        Each element may be NULL.
 */
static void	free_commands_range(t_command **cmds, int count)
{
	int	i;

	if (!cmds || count <= 0)
		return ;
	i = 0;
	while (i < count)
	{
		if (cmds[i])
			free_command(cmds[i]);
		cmds[i] = NULL;
		++i;
	}
}

/* -------------------------------------------------------------------------- */
/*                         Per-command execution data                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Frees all dynamically allocated members of t_execute_data.
 *
 * Does NOT free @p data itself (lifetime managed by caller).
 * Also unlinks a temporary heredoc file if present.
 */
void	free_execute_data(t_execute_data *data)
{
	if (data == NULL)
		return ;
	if (data->heredoc_file != NULL)
	{
		/* Best-effort unlink; ignore errors */
		unlink(data->heredoc_file);
		free(data->heredoc_file);
		data->heredoc_file = NULL;
	}
	free_ptr((void **)&data->input_file);
	free_ptr((void **)&data->output_file);
	if (data->path_dirs)
	{
		free_split(data->path_dirs);
		data->path_dirs = NULL;
	}
	if (data->original_args)
	{
		free_split(data->original_args);
		data->original_args = NULL;
	}
	if (data->clean_args)
	{
		free_split(data->clean_args);
		data->clean_args = NULL;
	}
	free_ptr((void **)&data->cmd_path);

	/* Note: data->env_list is owned by the process/shell, not freed here. */
}

/* -------------------------------------------------------------------------- */
/*                          Pipeline / process cleanup                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Recursively frees an array of t_command pointers.
 *        Kept for API compatibility; implemented iteratively internally.
 *
 * @param cmds  The array of t_command pointers to free.
 * @param index Current index (ignored; we free full array).
 * @param count Total number of commands in the array.
 */
void	free_commands_recursive(t_command **cmds, int index, int count)
{
	(void)index;
	free_commands_range(cmds, count);
}

/**
 * @brief Cleans up all resources used in pipeline execution (parent side).
 *
 * Frees PATH dirs, command structures, command array, and split parts.
 * Does NOT free env_list (owned by the shell).
 * Sets pointers to NULL and resets counters to avoid dangling references.
 */
void	cleanup_pipeline_resources(t_process_data *data)
{
	if (!data)
		return ;
	if (data->path_dirs)
	{
		free_split(data->path_dirs);
		data->path_dirs = NULL;
	}
	/* Free individual command objects, then the array */
	if (data->cmds)
	{
		free_commands_range(data->cmds, data->cmd_count);
		free_ptr((void **)&data->cmds);
		data->cmd_count = 0;
	}
	if (data->parts)
	{
		free_split(data->parts);
		data->parts = NULL;
	}
	/* pids are only used in parent; free them here if allocated */
	free_ptr((void **)&data->pids);
}

/**
 * @brief Cleans up all resources used by a child executing a pipeline.
 *
 * Frees commands, PATH dirs, PIDs array, env list, and split parts.
 * Also clears any static buffers from get_next_line.
 */
void	cleanup_child_pipeline_resources(t_process_data *data)
{
	if (!data)
		return ;
	get_next_line_cleanup();

	/* Free command objects and container */
	if (data->cmds)
	{
		free_commands_range(data->cmds, data->cmd_count);
		free_ptr((void **)&data->cmds);
		data->cmd_count = 0;
	}

	/* PATH dirs, PIDs, environment list, and parts */
	if (data->path_dirs)
	{
		free_split(data->path_dirs);
		data->path_dirs = NULL;
	}
	free_ptr((void **)&data->pids);

	if (data->env_list)
	{
		free_env(data->env_list);
		data->env_list = NULL;
	}
	if (data->parts)
	{
		free_split(data->parts);
		data->parts = NULL;
	}
}
