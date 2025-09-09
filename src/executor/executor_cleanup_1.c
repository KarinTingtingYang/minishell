/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:58:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:38:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees a range of t_command pointers in an array.
 * 
 * Sets each pointer to NULL after freeing.
 * @param cmds  The array of t_command pointers to free.
 * @param count The number of commands in the array.
 * @return void
 */
void	free_commands_range(t_command **cmds, int count)
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

/**
 * @brief Recursively frees t_command pointers in an array.
 *
 * This function uses recursion to free each command in the array one by one,
 * ensuring that all commands are properly freed even if the count is large.
 *
 * @param cmds  The array of t_command pointers to free.
 * @param index The current index in the recursion.
 * @param count The total number of commands in the array.
 */
void	free_commands_recursive(t_command **cmds, int index, int count)
{
	(void)index;
	free_commands_range(cmds, count);
}

/**
 * @brief Recursively frees t_command pointers in an array.
 *
 * This function uses recursion to free each command in the array one by one,
 * ensuring that all commands are properly freed even if the count is large.
 *
 * @param cmds  The array of t_command pointers to free.
 * @param index The current index in the recursion.
 * @param count The total number of commands in the array.
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
	free_ptr((void **)&data->pids);
}
