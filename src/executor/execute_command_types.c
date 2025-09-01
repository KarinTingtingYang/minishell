/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_types.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:24:59 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 19:28:58 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int execute_builtin_command(t_execute_data *data, t_process_data *process_data)
{
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int exit_status;

    if (apply_builtin_redirection(data->input_file, data->output_file, data->output_mode) == -1)
        exit_status = 1;
    else
        exit_status = run_builtin(data->clean_args, process_data);

    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    
    return exit_status;
}

int execute_external_command(t_execute_data *data, t_process_data *process_data, t_env_var *env_list)
{
    char **path_dirs = find_path_dirs(env_list);
    int exit_status;
    
    data->cmd_path = find_full_cmd_path(data->clean_args[0], path_dirs);
    free_split(path_dirs);
    
    exit_status = execute_prepared_command(data, process_data);
    return exit_status;
}

int handle_redirection_only(t_execute_data *data, t_process_data *process_data)
{
    int exit_status = 0;
    
    if (data->input_file)
    {
        int fd = open(data->input_file, O_RDONLY);
        if (fd == -1) {
            ft_error(data->input_file, strerror(errno));
            exit_status = 1;
        } else {
            close(fd);
        }
    }
    
    process_data->last_exit_status = exit_status;
    free_execute_data(data);
    return 0;
}

int execute_single_command(char **args, t_env_var *env_list, t_process_data *process_data)
{
    t_execute_data data;
    int exit_status;

    ft_bzero(&data, sizeof(t_execute_data));
    data.env_list = env_list;
    process_data->in_pipeline = 0;
    
    data.clean_args = handle_redirection(args, process_data, &data.input_file, 
        &data.output_file, &data.output_mode, &data.heredoc_file);
    
    if (!data.clean_args)
        return handle_redirection_error(&data, process_data, args);
        
    if (!data.clean_args[0])
        return handle_redirection_only(&data, process_data);

    if (is_builtin(data.clean_args[0]))
        exit_status = execute_builtin_command(&data, process_data);
    else
        exit_status = execute_external_command(&data, process_data, env_list);

    process_data->last_exit_status = exit_status;
    free_execute_data(&data);
    return exit_status;
}