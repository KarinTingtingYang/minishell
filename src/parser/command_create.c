/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_create.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/26 10:49:56 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/27 11:08:47 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Resolves the full path of the executable command.
 * 
 * Uses the first argument in the command (`cmd->args[0]`) and searches 
 * for its full path using the directories provided in `path_dirs`.
 * On success, sets `cmd->cmd_path` to the resolved full path.
 * @param cmd        Pointer to the command struct to update.
 * @param path_dirs  NULL-terminated array of directories from the PATH
 *                   environment variable.
 * @return           0 on success, -1 if the command was not found.
 */
static int	search_command_in_path(t_command *cmd, char **path_dirs)
{
	cmd->cmd_path = find_full_cmd_path(cmd->args[0], path_dirs);
	if (cmd->cmd_path == NULL)
		return (-1);
	return (0);
}

/**
 * @brief Frees the memory allocated for a command structure.
 * 
 * This function frees all dynamically allocated fields in the `t_command`
 * structure, including the command path, arguments, input/output files,
 * and heredoc file. It does not free the structure itself.
 * @param cmd Pointer to the command structure to free.
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
 * @brief Parses command arguments and handles redirection.
 * 
 * This function processes the provided tokens to extract command arguments
 * and redirection information. It updates the `cmd` structure with the parsed
 * arguments and redirection files.
 * @param cmd    Pointer to the command structure to populate.
 * @param tokens Array of strings representing the command and its arguments.
 * @return       0 on success, -1 if an error occurs (e.g., memory
 *               allocation failure).
 */
static int	parse_args_and_redirection(t_command *cmd, char **tokens, t_process_data *process_data)
{
	char	**original_args;

	if (tokens == NULL || tokens[0] == NULL)
		return (-1);
	original_args = duplicate_split(tokens);
	if (original_args == NULL)
		return (-1);
	cmd->args = handle_redirection(original_args, process_data, &cmd->input_file,
			&cmd->output_file, &cmd->output_mode, &cmd->heredoc_file);
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
 * Allocates memory for a new `t_command` structure and initializes its fields
 * to NULL or default values. This is used as a starting point for creating
 * commands from parsed input.
 * @return A pointer to the newly created `t_command` structure,
 *         or NULL on failure.
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
 * @brief Creates a new command structure from the provided tokens.
 * 
 * This function initializes a new `t_command` structure, parses the provided
 * tokens to extract command arguments and redirection information, and searches
 * for the command in the PATH directories. If successful, it returns the command
 * structure; otherwise, it returns NULL.
 * @param tokens     Array of strings representing the command and its arguments.
 * @param path_dirs  NULL-terminated array of directories from the PATH
 *                   environment variable.
 * @return           A pointer to the newly created t_command structure,
 *                   or NULL on failure.
 */
t_command	*create_command(char **tokens, char **path_dirs, t_process_data *process_data)
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
	// if (search_command_in_path(cmd, path_dirs) == -1)
	// {
	// 	free_command(cmd);
	// 	return (NULL);
	// }
	// if (!is_builtin(cmd->args[0]) && search_command_in_path(cmd, path_dirs) == -1) // DEBUGGING
    // {
    //     free_command(cmd);
    //     return (NULL);
    // }
	if (!is_builtin(cmd->args[0]))
		search_command_in_path(cmd, path_dirs);
	return (cmd);
}
