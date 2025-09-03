/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_pipeline_helper.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 11:30:14 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 11:40:20 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int setup_command(char **expanded_args, t_process_data *data, int index)
{
    data->cmds[index] = create_command(expanded_args, data->path_dirs, data);
    free_split(expanded_args);
    if (data->cmds[index] == NULL)
    {
        if (g_signal_received == SIGINT)
            return (0);
        ft_error(NULL, "command creation failed");
        return (0);
    }
    return (1);
}

static char **expand_command_args(char *cmd_str, t_process_data *data)
{
    t_token **tokens = parse_line(cmd_str);
    char **expanded_args;

    if (tokens == NULL)
        return (NULL);
    expanded_args = expand_and_split_args(tokens, data->env_list, 
                                        data->last_exit_status);
    free_tokens(tokens);
    return (expanded_args);
}

static int build_single_command(char **parts, t_process_data *data, int index)
{
    char **expanded_args;

    expanded_args = expand_command_args(parts[index], data);
    if (expanded_args == NULL)
        return (0);
    return (setup_command(expanded_args, data, index));
}

static int initialize_pipeline_build(t_process_data *data)
{
    data->path_dirs = find_path_dirs(data->env_list);
    if (data->path_dirs == NULL)
    {
        ft_error(NULL, "PATH variable not found");
        return (0);
    }
    return (1);
}

int build_commands_from_parts(char **parts, int count, t_process_data *data)
{
    int index;

    if (!initialize_pipeline_build(data))
        return (0);
    data->in_pipeline = (count > 1);
    index = 0;
    while (index < count)
    {
        if (!build_single_command(parts, data, index))
        {
            free_split(data->path_dirs);
            return (0);
        }
        index++;
    }
    free_split(data->path_dirs);
    return (1);
}