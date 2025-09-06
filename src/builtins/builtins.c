/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:00:14 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/06 15:03:42 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a built-in command.
 *
 * @param args The command and its arguments.
 * @param process_data The process data containing environment variables and
 * last exit status.
 * @return The exit status of the built-in command, or 0 if not a built-in.
 */
int	run_builtin(char **args, t_process_data *process_data)
{
	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (run_echo(args, process_data->env_list), 0);
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (run_cd(args, process_data->env_list));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (run_pwd());
	if (ft_strncmp(args[0], "exit", 5) == 0)
		//return (run_exit(args, process_data->last_exit_status));
		// DEBUG
	{
		cleanup_child_pipeline_resources(process_data);
		return (run_exit(args, process_data->last_exit_status));
	}
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (run_env(process_data->env_list), 0);
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (run_export(process_data->env_list, args));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (run_unset(&(process_data->env_list), args));
	return (0);
}

/**
 * @brief Checks if a command is a built-in.
 *
 * @param cmd The command to check.
 * @return 1 if the command is a built-in, 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0);
}
