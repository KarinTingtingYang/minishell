/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:07:14 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 19:49:53 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void handle_stat_error(char **envp, char **args, int error_code)
{
    if (envp) 
        free_split(envp);
    if (error_code == ENOTDIR)
        ft_error_and_exit(args[0], "Not a directory", 126);
    else if (error_code == ENOENT)
        ft_error_and_exit(args[0], "No such file or directory", 127);
    ft_error_and_exit(args[0], strerror(error_code), 126);
}

void handle_access_error(char **envp, char **args, int error_code)
{
    if (envp) 
        free_split(envp);
    if (error_code == EACCES)
        ft_error_and_exit(args[0], "Permission denied", 126);
    else if (error_code == ENOTDIR)
        ft_error_and_exit(args[0], "Not a directory", 126);
    ft_error_and_exit(args[0], strerror(error_code), 126);
}

void handle_execve_error(char **envp, char **args, int error_code)
{
    free_split(envp);
    if (error_code == ENOEXEC)
        ft_error_and_exit(args[0], "Exec format error", 126);
    else if (error_code == ENOTDIR)
        ft_error_and_exit(args[0], "Not a directory", 126);
    else if (error_code == ENOENT)
        ft_error_and_exit(args[0], "No such file or directory", 127);
    else if (error_code == EACCES)
        ft_error_and_exit(args[0], "Permission denied", 126);
    ft_error_and_exit(args[0], strerror(error_code), 126);
}

void handle_null_cmd_path(char **args, char **envp)
{
    if (args && args[0] && ft_strchr(args[0], '/'))
    {
        if (envp) 
            free_split(envp);
        ft_error_and_exit(args[0], "No such file or directory", 127);
    }
	if (envp) 
		free_split(envp);
	if (args && args[0])
		ft_error_and_exit(args[0], "command not found", 127);
	else
		ft_error_and_exit("minishell", "command not found", 127);
}

int handle_redirection_error(t_execute_data *data, t_process_data *process_data,
	char **args)
{
	(void)data;
    if (g_signal_received == SIGINT)
    {
        process_data->last_exit_status = 130;
        return 130;
    }

    int last_arg_index = 0;
    while(args[last_arg_index + 1] != NULL) 
        last_arg_index++;
        
    if (is_redirection(args[last_arg_index]))
    {
        process_data->last_exit_status = 2;
        return 2;
    }
    process_data->last_exit_status = 1;
    return 1;
}