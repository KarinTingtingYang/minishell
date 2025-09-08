/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:58:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 10:48:30 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees all dynamically allocated members of t_execute_data.
 *
 * This function checks each member of the t_execute_data structure and
 * frees it if it is not NULL. It also sets the pointers to NULL after freeing
 * to avoid dangling pointers. If a heredoc file was created, it unlinks
 * (deletes) the file before freeing its name.
 *
 * @param data Pointer to the t_execute_data structure to free.
 */
void	free_execute_data(t_execute_data *data)
{
	if (data == NULL)
		return ;
	if (data->heredoc_file != NULL)
	{
		unlink(data->heredoc_file);
		free(data->heredoc_file);
	}
	free(data->input_file);
	free(data->output_file);
	free_split(data->path_dirs);
	free_split(data->original_args);
	free_split(data->clean_args);
	free(data->cmd_path);
	data->path_dirs = NULL;
	data->original_args = NULL;
	data->clean_args = NULL;
	data->cmd_path = NULL;
	data->heredoc_file = NULL;
	data->input_file = NULL;
	data->output_file = NULL;
}

/**
 * @brief Recursively frees an array of t_command pointers.
 *
 * This function frees each t_command in the array by calling free_command
 * and then frees the array itself. It uses recursion to iterate through
 * the array.
 *
 * @param cmds The array of t_command pointers to free.
 * @param index The current index in the recursion.
 * @param count The total number of commands in the array.
 */
void	free_commands_recursive(t_command **cmds, int index, int count)
{
	if (index >= count)
		return ;
	free_command(cmds[index]);
	free_commands_recursive(cmds, index + 1, count);
}

/**
 * @brief Cleans up all resources used in pipeline execution.
 *
 * This function frees the path directories, command structures,
 * command array, and split command parts used in pipeline execution.
 *
 * @param cmds Pointer to the array of t_command pointers.
 * @param parts The array of command parts split by pipes.
 * @param path_dirs The array of directories in the PATH environment variable.
 * @param count The number of commands in the pipeline.
 */
void	cleanup_pipeline_resources(t_process_data *data)
{
	if (data->path_dirs)
		free_split(data->path_dirs);
	free_commands_recursive(data->cmds, 0, data->cmd_count);
	free(data->cmds);
	free_split(data->parts);
}

void	cleanup_child_pipeline_resources(t_process_data *data)
{
	free_commands_recursive(data->cmds, 0, data->cmd_count);
	free(data->cmds);
	free_split(data->path_dirs);
	free(data->pids);
	free_env(data->env_list);
	free_split(data->parts);
}
