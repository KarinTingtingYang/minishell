/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command_execution.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:25:35 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/08 11:35:55 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Tokenizes the input line.
 *
 * @param line The command line input to be tokenized.
 * @param data Pointer to `t_execute_data` where tokenized data is stored.
 * @return Returns 0 if the command is empty or invalid, 1 otherwise.
 */
static int	tokenize_and_check(char *line, t_execute_data *data,
									t_process_data *pd)
{
	if (line == NULL || *line == '\0')
		return (0);
	data->original_args = tokenize_input(line, pd);
	if (data->original_args == NULL)
	{
		return (0);
	}
	if (data->original_args[0] == NULL)
	{
		free_split(data->original_args);
		data->original_args = NULL;
		return (0);
	}
	return (1);
}

/**
 * @brief Handles input/output redirection.
 *
 * This function processes the original arguments to handle any
 * input/output redirection symbols and files. It updates the
 * `t_execute_data` structure with cleaned arguments and redirection
 * information.
 * @param data Pointer to `t_execute_data` where redirection data is stored.
 * @param process_data Pointer to the global process data.
 * @return Returns 0 if there was a redirection syntax or file error,
 *         1 otherwise.
 */
static int	handle_redirections(t_execute_data *data,
	t_process_data *process_data)
{
	data->clean_args = handle_redirection(data->original_args,
			process_data, data);
	if (!data->clean_args)
		return (0);
	return (1);
}

/**
 * @brief Finds the command path.
 *
 * @param env_list The environment variables (used to extract PATH).
 * @param data Pointer to `t_execute_data` where command path is stored.
 * @return Returns 127 if the command was not found in PATH, 1 otherwise.
 */
static int	find_command_path(t_env_var *env_list, t_execute_data *data)
{
	if (is_builtin(data->clean_args[0]))
	{
		data->cmd_path = NULL;
		data->path_dirs = find_path_dirs(env_list);
		return (1);
	}
	data->path_dirs = find_path_dirs(env_list);
	if (!data->path_dirs)
	{
		ft_error("", "PATH variable not found");
		return (1);
	}
	data->cmd_path = find_full_cmd_path(data->clean_args[0], data->path_dirs);
	if (!data->cmd_path)
		return (127);
	data->env_list = env_list;
	return (1);
}

/**
 * @brief Prepares the data required to execute a single shell command.
 *
 * This function splits the input line into arguments, handles input/output
 * redirection, searches for the command in the PATH, and stores the
 * execution-related information in a `t_execute_data` structure.
 * It performs the following:
 * - Splits the line into `original_args`.
 * - Processes and removes redirection symbols and stores cleaned arguments in
 *   `clean_args`.
 * - Extracts input/output file redirection if present.
 * - Resolves the full command path using the PATH environment variable.
 * If any of these steps fail, 
 * it cleans up partially allocated resources and returns
 * a specific code to indicate the error or status.
 * @param line The command line input to be processed.
 * @param envp The environment variables (used to extract PATH).
 * @param data Pointer to `t_execute_data` where parsed 
 * and prepared data is stored.
 * @return Returns:
 *         - 0 if the command is empty or invalid.
 *         - 1 on successful preparation.
 *         - 2 if there is a redirection syntax or file error.
 *         - 127 if the command was not found in PATH.
 */
int	prepare_command_execution(char *line, t_env_var *env_list,
	t_execute_data *execute_data, t_process_data *process_data)
{
	int	status;

	execute_data->heredoc_file = NULL;
	if (!tokenize_and_check(line, execute_data, process_data))
		return (0);
	if (!handle_redirections(execute_data, process_data))
		return (2);
	if (!execute_data->clean_args[0])
		return (0);
	status = find_command_path(env_list, execute_data);
	return (status);
}
