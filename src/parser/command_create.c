/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:49:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 10:03:32 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Searches for the command in the PATH directories.
 * 
 * This function attempts to find the full path of the command specified
 * in `cmd->args[0]` by searching through the provided `path_dirs`.
 * If found, it sets `cmd->cmd_path` to the full path; otherwise, it
 * leaves it as NULL.
 * @param cmd       Pointer to the command structure to update.
 * @param path_dirs NULL-terminated array of directories from the PATH
 *                  environment variable.
 * @return          0 if the command is found, -1 if not found.
 */
static int	search_command_in_path(t_command *cmd, char **path_dirs)
{
	cmd->cmd_path = find_full_cmd_path(cmd->args[0], path_dirs);
	if (cmd->cmd_path == NULL)
		return (-1);
	return (0);
}

/**
 * @brief Duplicates an array of strings.
 * 
 * This function creates a deep copy of the provided NULL-terminated
 * array of strings. Each string is duplicated, and the new array is
 * also NULL-terminated.
 * @param split The original array of strings to duplicate.
 * @return      A pointer to the newly allocated array of strings,
 *              or NULL on failure.
 */
static char	**duplicate_split(char **split)
{
	int		i;
	char	**copy;

	if (split == NULL)
		return (NULL);
	i = 0;
	while (split[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (split[i])
	{
		copy[i] = strdup(split[i]);
		if (copy[i] == NULL)
		{
			while (--i >= 0)
				free(copy[i]);
			return (free(copy), NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/**
 * @brief Parses command arguments and redirections from tokens.
 * 
 * This function processes the provided tokens to separate command
 * arguments from redirection operators and their associated files.
 * It updates the command structure with the cleaned arguments and
 * redirection information.
 * @param cmd          Pointer to the command structure to update.
 * @param tokens       Array of strings representing the command and its
 *                     arguments, including redirection operators.
 * @param process_data Pointer to the process data structure for context.
 * @return             -1 on memory allocation failure or error,
 *                     1 if no command arguments remain after parsing,
 *                     0 on successful parsing with valid command arguments.
 */
static int	parse_args_and_redirection(t_command *cmd, char **tokens,
											t_process_data *process_data)
{
	char	**original_args;

	if (tokens == NULL || tokens[0] == NULL)
		return (-1);
	original_args = duplicate_split(tokens);
	if (original_args == NULL)
		return (-1);
	cmd->args = handle_redirection(original_args, process_data,
			(t_execute_data *)cmd);
	free_split(original_args);
	if (cmd->args == NULL)
		return (-1);
	if (cmd->args[0] == NULL)
		return (1);
	return (0);
}

/**
 * @brief Creates an empty command structure.
 * 
 * This function allocates and initializes a new `t_command` structure
 * with all fields set to NULL or default values.
 * @return A pointer to the newly created `t_command` structure,
 *         or NULL on memory allocation failure.
 */
static t_command	*create_empty_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL;
	cmd->cmd_path = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->output_mode = 0;
	cmd->heredoc_file = NULL;
	return (cmd);
}

/**
 * @brief Creates and initializes a command structure from tokens.
 * 
 * This function creates a `t_command` structure, parses the provided
 * tokens to separate command arguments and redirections, and searches
 * for the command in the PATH if it's not a built-in.
 * @param tokens       Array of strings representing the command and its
 *                     arguments, including redirection operators.
 * @param path_dirs    NULL-terminated array of directories from the PATH
 *                     environment variable.
 * @param process_data Pointer to the process data structure for context.
 * @return             A pointer to the initialized `t_command` structure,
 *                     or NULL on failure. If no command arguments remain
 *                     after parsing, returns a command with args[0] == NULL.
 */
t_command	*create_command(char **tokens, char **path_dirs,
								t_process_data *process_data)
{
	t_command	*cmd;
	int			redir_parse_result;

	cmd = create_empty_command();
	if (cmd == NULL)
	{
		return (NULL);
	}
	redir_parse_result = parse_args_and_redirection(cmd, tokens, process_data);
	if (redir_parse_result == -1)
	{
		free(cmd);
		return (NULL);
	}
	if (redir_parse_result == 1)
		return (cmd);
	if (!is_builtin(cmd->args[0]))
		search_command_in_path(cmd, path_dirs);
	return (cmd);
}
