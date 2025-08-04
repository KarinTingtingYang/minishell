/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 08:58:09 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/04 10:29:58 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_execute_data(t_execute_data *data)
{
    if (data == NULL)
        return ;
	 // Add this block to clean up the heredoc file
    if (data->heredoc_file)
    {
        unlink(data->heredoc_file);
        free(data->heredoc_file);
    }

    free_split(data->path_dirs);
    free_split(data->original_args);
    free_split(data->clean_args);
    free(data->cmd_path);
    data->path_dirs = NULL;
    data->original_args = NULL;
    data->clean_args = NULL;
    data->cmd_path = NULL;
}

void free_commands_recursive(t_command **cmds, int index, int count)
{
    if (index >= count)
        return ;
    free_command(cmds[index]);
    free_commands_recursive(cmds, index + 1, count);
}

int count_command_parts(char **parts)
{
    int count = 0;
    while (parts[count] != NULL)
        count++;
    return (count);
}