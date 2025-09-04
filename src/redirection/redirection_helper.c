/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:36:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 09:34:49 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles an output append redirection.
 *
 * @param args The token list.
 * @param i The index of the redirection token.
 * @param exec_data A pointer to the t_execute_data struct.
 * @return 0 on success, -1 on a file creation/access error.
 */
static int	handle_output_append(char **args, int i, t_execute_data *exec_data)
{
	if (process_output_file(args[i + 1], 1) != 0)
		return (-1);
	free(exec_data->output_file);
	exec_data->output_file = ft_strdup(args[i + 1]);
	exec_data->output_mode = 2;
	return (0);
}

/**
 * @brief Handles an input redirection.
 *
 * @param args The token list.
 * @param i The index of the redirection token.
 * @param process_data A pointer to the global process data.
 * @param exec_data A pointer to the t_execute_data struct.
 * @return 0 on success, -1 on a file access error.
 */
static int	handle_input_redirection(char **args, int i,
						t_process_data *process_data, t_execute_data *exec_data)
{
	free(exec_data->input_file);
	exec_data->input_file = ft_strdup(args[i + 1]);
	if (process_data->in_pipeline == 0)
	{
		if (access(exec_data->input_file, F_OK) != 0)
		{
			ft_error(args[i + 1], "No such file or directory");
			return (-1);
		}
		if (access(exec_data->input_file, R_OK) != 0)
		{
			ft_error(args[i + 1], "Permission denied");
			return (-1);
		}
	}
	return (0);
}

/**
 * @brief Handles an output truncation redirection.
 *
 * @param args The token list.
 * @param i The index of the redirection token.
 * @param exec_data A pointer to the t_execute_data struct.
 * @return 0 on success, -1 on a file creation/access error.
 */
static int	handle_output_truncation(char **args, int i,
										t_execute_data *exec_data)
{
	if (process_output_file(args[i + 1], 0) != 0)
		return (-1);
	free(exec_data->output_file);
	exec_data->output_file = ft_strdup(args[i + 1]);
	exec_data->output_mode = 1;
	return (0);
}

/**
 * @brief Handles a heredoc redirection.
 *
 * @param args The token list.
 * @param i The index of the redirection token.
 * @param process_data A pointer to the global process data.
 * @param exec_data A pointer to the t_execute_data struct.
 * @return 0 on success, -1 on a heredoc creation failure.
 */
static int	handle_heredoc_redirection(char **args, int i,
				t_process_data *process_data, t_execute_data *exec_data)
{
	if (exec_data->heredoc_file)
	{
		unlink(exec_data->heredoc_file);
		free(exec_data->heredoc_file);
		exec_data->heredoc_file = NULL;
	}
	exec_data->heredoc_file = handle_heredoc(args[i + 1],
			process_data->env_list, process_data->last_exit_status);
	if (exec_data->heredoc_file == NULL)
		return (-1);
	free(exec_data->input_file);
	exec_data->input_file = ft_strdup(exec_data->heredoc_file);
	return (0);
}

/**
 * @brief Processes a single redirection token and its corresponding file.
 *
 * This function validates the redirection syntax and calls the appropriate
 * helper function to handle the specific redirection type.
 *
 * @param args The token list.
 * @param i The index of the redirection token.
 * @param process_data A pointer to the global process data.
 * @param exec_data A pointer to the t_execute_data struct
 *                  containing redirection files.
 * @return 0 on success, -1 on a syntax error or file access issue.
 */
int	process_redirection_token(char **args, int i, t_process_data *process_data,
								t_execute_data *exec_data)
{
	char	*msg;
	char	*full_msg;

	if (args[i + 1] == NULL)
	{
		ft_error("", "syntax error near unexpected token `newline'");
		return (-1);
	}
	if (is_redirection(args[i + 1]))
	{
		msg = ft_strjoin("syntax error near unexpected token `", args[i + 1]);
		full_msg = ft_strjoin(msg, "'");
		ft_error("", full_msg);
		free(msg);
		return (free(full_msg), -1);
	}
	if (ft_strncmp(args[i], "<<", 3) == 0)
		return (handle_heredoc_redirection(args, i, process_data, exec_data));
	else if (ft_strncmp(args[i], "<", 2) == 0)
		return (handle_input_redirection(args, i, process_data, exec_data));
	else if (ft_strncmp(args[i], ">", 2) == 0)
		return (handle_output_truncation(args, i, exec_data));
	else if (ft_strncmp(args[i], ">>", 3) == 0)
		return (handle_output_append(args, i, exec_data));
	return (0);
}
