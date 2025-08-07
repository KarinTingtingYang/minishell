/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:49:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 13:23:39 by makhudon         ###   ########.fr       */
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
static int search_command_in_path(t_command *cmd, char **path_dirs)
{
    cmd->cmd_path = find_full_cmd_path(cmd->args[0], path_dirs);
    if (cmd->cmd_path == NULL)
        return (-1);
    return (0);
}

/**
 * @brief Duplicates a NULL-terminated array of strings.
 * 
 * Allocates memory for a new array and copies each string from the original
 * array to the new one. The new array is also NULL-terminated.
 * @param split  The original array of strings to duplicate.
 * @return       A new NULL-terminated array of strings, or NULL on failure.
 */
static char **duplicate_split(char **split)
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
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/**
 * @brief Parses the command arguments and handles redirection.
 * 
 * This function takes a command structure and an array of tokens,
 * parses the tokens to extract command arguments, and handles any
 * input/output redirection specified in the tokens.
 * If parsing fails, it returns -1; if only redirection is present,
 * it returns 1; otherwise, it returns 0 on success.
 * @param cmd    Pointer to the command structure to fill.
 * @param tokens Array of strings representing the command and its arguments.
 * @return       0 on success, -1 on failure, 1 if only redirection is present.
 */
static int parse_args_and_redirection(t_command *cmd, char **tokens)
{
    char **original_args;
	
	if (tokens == NULL || tokens[0] == NULL) // no command to execute
        return (-1);
    original_args = duplicate_split(tokens);
    if (original_args == NULL)
        return (-1);
    cmd->args = handle_redirection(original_args, &cmd->input_file, 
		&cmd->output_file, &cmd->output_mode, &cmd->heredoc_file);
    free_split(original_args);
    if (cmd->args == NULL)
        return (-1);
    if (cmd->args[0] == NULL)
        return (1);
    return (0);
}

/**
 * @brief Allocates and initializes a new t_command structure.
 * 
 * This function allocates memory for a t_command struct and initializes all
 * of its fields (`args`, `cmd_path`, `input_file`, and `output_file`) to NULL.
 * It is typically used as the first step in preparing a command for execution.
 * @return A pointer to the newly allocated and initialized t_command structure,
 *         or NULL if memory allocation fails.
 */
static t_command *create_empty_command(void)
{
	t_command *cmd;
	
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
 * @brief Creates a t_command structure from an array of tokens.
 * 
 * This function initializes a new t_command structure, parses the tokens
 * to extract command arguments and redirection information, and resolves
 * the command path using the provided PATH directories.
 * If any step fails, it cleans up and returns NULL.
 * @param tokens    Array of strings representing the command and its arguments.
 * @param path_dirs NULL-terminated array of directories from the PATH environment variable.
 * @return A pointer to the newly created t_command structure, or NULL on failure.
 */
t_command *create_command(char **tokens, char **path_dirs)
{
    t_command *cmd;
	int redir_parse_result;
	
	cmd = create_empty_command();
    if (cmd == NULL)
	{
        return (NULL);
	}
	redir_parse_result = parse_args_and_redirection(cmd, tokens);
    if (redir_parse_result == -1)
    {
        free(cmd);
        return (NULL);
    }
    if (redir_parse_result == 1) // no executable command, just redirection
        return (cmd);
    if (search_command_in_path(cmd, path_dirs) == -1)
    {
        free_command(cmd);
        return (NULL);
    }
    return (cmd);
}
