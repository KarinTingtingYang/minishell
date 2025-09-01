/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prepared_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:54:16 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 16:55:41 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the execution of a builtin command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the builtin command.
 */
static int execute_builtin(t_execute_data *data, t_process_data *process_data)
{
    run_builtin(data->clean_args, process_data);
    free_execute_data(data);
    return (0);
}

/**
 * @brief Forks the process and executes the command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
static int fork_and_execute(t_execute_data *data, t_process_data *process_data)
{
    pid_t pid;
    int exit_code;

    signal(SIGINT, SIG_IGN);

    pid = fork();
    if (pid < 0)
    {
        ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
        free_execute_data(data);
        return (-1);
    }
    else if (pid == 0)
    {
        redirect_io(data->input_file, data->output_file, data->output_mode);
        if (is_builtin(data->clean_args[0]))
            run_builtin(data->clean_args, process_data);
        else
            execute_cmd(data->cmd_path, data->clean_args, data->path_dirs, data->env_list);
        exit(0);
    }
    else
    {
        exit_code = wait_for_child_and_handle_status(pid);
        setup_signal_handlers();
        process_data->last_exit_status = exit_code;
        free_execute_data(data);
        return (exit_code);
    }
}

/**
 * @brief Executes a previously prepared command.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
int execute_prepared_command(t_execute_data *data, t_process_data *process_data)
{
    int has_redirection = (data->input_file || data->output_file);

    if (is_builtin(data->clean_args[0]) && !has_redirection && !process_data->in_pipeline)
        return execute_builtin(data, process_data);

    return fork_and_execute(data, process_data);
}