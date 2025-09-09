/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 13:38:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:41:36 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a command using execve after performing necessary checks.
 *
 * This function prepares the environment, validates the command path,
 * and executes the command. It handles errors appropriately and
 * ensures resources are freed in case of failure.
 *
 * @param cmd_path The full path to the command to execute.
 * @param args The argument list for the command (argv-style).
 * @param path_dirs The directories in PATH (not used here).
 * @param env_list The linked list of environment variables.
 */
void	execute_cmd(char *cmd_path, char **args, char **path_dirs,
	t_env_var *env_list)
{
	char	**envp;
	int		e;

	(void)path_dirs;
	reset_child_signal_handlers();
	envp = env_list_to_array(env_list);
	if (cmd_path == NULL)
	{
		handle_null_cmd_path(args, envp);
		return ;
	}
	if (!perform_command_checks(cmd_path, args, envp))
		return ;
	execve(cmd_path, args, envp);
	e = errno;
	handle_execve_error(envp, args, e);
}

/**
 * @brief Handles execution of a single command line.
 * Parses, expands, and executes the command.
 * Frees all resources before returning.
 * @param line The command line to execute.
 * @param env_list The linked list of environment variables.
 * @param process_data Pointer to the process data.
 * @return The exit status of the command, or 0 if no command was executed.
 */
int	handle_single_command(char *line, t_env_var *env_list,
								t_process_data *process_data)
{
	t_token	**tokens;
	char	**args;
	int		result;
	int		prev;

	tokens = parse_line(line, process_data);
	if (tokens == NULL)
	{
		if (process_data != NULL)
			return (process_data->last_exit_status);
		return (0);
	}
	args = expand_and_split_args(tokens, process_data);
	free_tokens(tokens);
	if (process_data && process_data->syntax_error)
		return (free_split(args), process_data->last_exit_status);
	if (args == NULL || args[0] == NULL)
	{
		prev = 0;
		if (process_data)
			prev = process_data->last_exit_status;
		return (free_split(args), prev);
	}
	result = execute_single_command(args, env_list, process_data);
	return (free_split(args), result);
}
