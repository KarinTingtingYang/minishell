/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:58:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 09:40:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

void free_execute_data(t_execute_data *data)
{
    if (data == NULL)
        return ;
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
