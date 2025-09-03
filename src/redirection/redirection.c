/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:25:34 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/03 15:53:46 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Opens and redirects input from a file.
 *
 * This function opens the specified input file for reading and
 * redirects the standard input (stdin) to read from this file.
 * If the file cannot be opened, it prints an error message and
 * exits the process.
 *
 * @param input_file The name of the input file to open.
 */
void	redirect_io(char *input_file, char *output_file, int output_mode)
{
	if (input_file)
		open_and_redirect_input(input_file);
	if (output_file)
		open_and_redirect_output(output_file, output_mode);
}

/**
 * @brief Builds a new array containing only the clean arguments.
 *
 * @param args The original token array.
 * @param argc The count of clean arguments.
 * @return A new null-terminated array of strings,
 *         or NULL on memory allocation failure.
 */
static char	**build_clean_args(char **args, int argc)
{
	char	**clean_args;
	int		i;
	int		j;

	clean_args = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	j = 0;
	if (clean_args == NULL)
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	while (args[i] != NULL)
	{
		if (is_redirection(args[i]))
			i += 2;
		else
			clean_args[j++] = ft_strdup(args[i++]);
	}
	clean_args[j] = NULL;
	return (clean_args);
}

/**
 * @brief Processes all redirection tokens in the argument list.
 *
 * This function iterates through the argument list, identifies redirection
 * tokens, and processes them using the appropriate helper functions. It also
 * counts the number of valid command arguments (non-redirection tokens).
 *
 * @param args The token list.
 * @param process_data A pointer to the global process data.
 * @param exec_data A pointer to the t_execute_data struct
 *                  containing redirection files.
 * @return The count of valid command arguments, or -1 on
 *         a syntax error or file access issue.
 */
static int	process_all_redirections_loop(char **args,
					t_process_data *process_data, t_execute_data *exec_data)
{
	int	i;
	int	argc;

	i = 0;
	argc = 0;
	while (args[i] != NULL)
	{
		if (is_redirection(args[i]))
		{
			if (process_redirection_token(args, i,
					process_data, exec_data) != 0)
			{
				return (-1);
			}
			i += 2;
		}
		else
		{
			argc++;
			i++;
		}
	}
	return (argc);
}

/**
 * @brief Parses args for multiple redirections and
 *        returns a clean command array.
 *
 * This function handles all I/O redirection parsing,
 * populating the `t_execute_data` struct with the final
 * input/output files and mode.
 *
 * @param args The original token array.
 * @param process_data A struct containing process-specific data.
 * @param exec_data A pointer to the `t_execute_data` struct to be populated.
 * @return A new, null-terminated array of strings containing only the command
 * and its arguments. Returns NULL on syntax or file error.
 */
char	**handle_redirection(char **args, t_process_data *process_data,
								t_execute_data *exec_data)
{
	int	argc;

	exec_data->input_file = NULL;
	exec_data->output_file = NULL;
	exec_data->output_mode = 0;
	exec_data->heredoc_file = NULL;
	argc = process_all_redirections_loop(args, process_data, exec_data);
	if (argc == -1)
	{
		if (exec_data->heredoc_file)
		{
			unlink(exec_data->heredoc_file);
			free(exec_data->heredoc_file);
			exec_data->heredoc_file = NULL;
		}
		free(exec_data->input_file);
		free(exec_data->output_file);
		exec_data->input_file = NULL;
		exec_data->output_file = NULL;
		return (NULL);
	}
	return (build_clean_args(args, argc));
}
