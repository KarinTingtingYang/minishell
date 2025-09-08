/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 10:52:18 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char	**environ;

/**
 * @brief Handles errors from the stat() system call.
 *
 * This function checks the error code and prints an appropriate error message
 * before exiting with a specific status code.
 *
 * @param envp The environment variables array.
 * @param args The command arguments array.
 * @param e The error code from errno.
 */
int	perform_command_checks(char *cmd_path, char **args, char **envp)
{
	struct stat	st;
	int			e;

	if (stat(cmd_path, &st) == -1)
	{
		e = errno;
		handle_stat_error(envp, args, e);
		return (0);
	}
	if (S_ISDIR(st.st_mode))
	{
		if (envp)
			free_split(envp);
		ft_error_and_exit(args[0], "Is a directory", 126);
		return (0);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		e = errno;
		handle_access_error(envp, args, e);
		return (0);
	}
	return (1);
}

/**
 * @brief Executes a command using execve().
 *
 * This function sets up the child process environment, checks for command
 * validity, and executes the command. If any step fails, it handles the error
 * appropriately.
 *
 * @param cmd_path The full path to the command executable.
 * @param args The command arguments array.
 * @param path_dirs The array of directories in the PATH environment variable.
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
 * @brief Handles the execution of a single command line.
 *
 * This function parses the command line, expands arguments, and executes
 * the command. It returns the exit status of the command execution.
 *
 * @param line The command line string.
 * @param env_list The linked list of environment variables.
 * @param process_data Pointer to the process data structure.
 * @return The exit status of the command execution.
 */
int	handle_single_command(char *line, t_env_var *env_list,
	t_process_data *process_data)
{
	t_token		**tokens;
	char		**args;
	int			result;

	tokens = parse_line(line);
	if (tokens == NULL)
		return (1);
	args = expand_and_split_args(tokens, process_data);
	free_tokens(tokens);
	if (process_data->syntax_error)
	{
		free_split(args);
		return (process_data->last_exit_status);
	}
	if (args == NULL || args[0] == NULL)
	{
		free_split(args);
		return (0);
	}
	result = execute_single_command(args, env_list, process_data);
	free_split(args);
	return (result);
}

/**
 * @brief Handles the execution of a pipeline command line.
 *
 * This function prepares the pipeline commands, retrieves the PATH
 * directories, and executes the command pipeline. It cleans up resources
 * afterward and returns the exit status of the pipeline execution.
 *
 * @param line The command line string containing pipes.
 * @param env_list The linked list of environment variables.
 * @param process_data Pointer to the process data structure.
 * @return The exit status of the pipeline execution.
 */
// DEBUG
int	handle_pipeline_command(char *line, t_env_var *env_list,
	t_process_data *process_data)
{
	int			status;

	process_data->parts = NULL;
	process_data->cmds = prepare_pipeline_commands(line,
			&process_data->cmd_count, &process_data->parts, process_data);
	if (process_data->syntax_error)
		return (free_split(process_data->parts),
			process_data->last_exit_status);
	if (process_data->cmds == NULL)
	{
		if (g_signal_received == SIGINT)
		{
			process_data->last_exit_status = 130;
			return (130);
		}
		process_data->last_exit_status = 2;
		return (2);
	}
	process_data->path_dirs = find_path_dirs(env_list);
	status = run_command_pipeline(process_data);
	cleanup_pipeline_resources(process_data);
	return (process_data->last_exit_status = status, status);
}

/**
 * @brief Executes a command line, handling both single and pipeline commands.
 *
 * This function checks for heredoc limits, determines if the command line
 * contains pipes, and delegates execution to the appropriate handler.
 *
 * @param line The command line string.
 * @param env_list The linked list of environment variables.
 * @param process_data Pointer to the process data structure.
 * @return The exit status of the command execution, or -1 on error.
 */
int	execute_command(char *line, t_env_var *env_list,
	t_process_data *process_data)
{
	if (!check_heredoc_limit(line))
		return (2);
	if (is_unquoted_pipe_present(line))
		return (handle_pipeline_command(line, env_list, process_data));
	else
		return (handle_single_command(line, env_list, process_data));
}
