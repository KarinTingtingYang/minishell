/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:49:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/26 11:46:30 by makhudon         ###   ########.fr       */
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
 * @brief Parses a command line string into arguments and handles redirection.
 * 
 * This function splits the input line into tokens using spaces, extracts any
 * redirection operators (`<`, `>`) and their associated file paths, and stores
 * them in the given `t_command` structure. It updates `cmd->args` with a cleaned
 * argument list (without redirection tokens), and sets `cmd->input_file` and
 * `cmd->output_file` accordingly.
 * Special return values:
 * - Returns  0 on success with valid arguments and optional redirections.
 * - Returns  1 if the command is only a redirection with no executable
 *   (e.g., `> out.txt`).
 * - Returns -1 on error, such as memory allocation failure or redirection
 *   syntax error.
 * @param cmd  Pointer to the command structure to populate.
 * @param line Raw input command line string to parse.
 * @return     0 on success, 1 if only redirection, -1 on failure.
 */
static int parse_args_and_redirection(t_command *cmd, char *line)
{
    char **original_args;
	
	original_args = ft_split(line, ' ');
    if (original_args == NULL || original_args[0] == NULL)
    {
        free_split(original_args);
        return (-1);
    }
    cmd->args = handle_redirection(original_args, &cmd->input_file, &cmd->output_file);
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
	return (cmd);
}

/**
 * @brief Creates and initializes a command structure from a command line string.
 * 
 * This function initializes a new t_command structure, parses the command line
 * to separate arguments and handle input/output redirection, and resolves the
 * command's executable path using the provided list of path directories.
 * If parsing fails or the command path cannot be resolved, it frees allocated
 * resources and returns NULL.
 * If the command line contains only redirections without an executable command,
 * it returns the command structure with redirection fields set.
 * @param line       The input command line string to parse.
 * @param path_dirs  An array of directory strings representing the system PATH.
 * @return A pointer to a fully initialized t_command structure on success,
 *         or NULL on failure.
 */
t_command *create_command(char *line, char **path_dirs)
{
    t_command *cmd;
	int redir_parse_result;
	
	cmd = create_empty_command();
    if (cmd == NULL)
	{
        return (NULL);
	}
	redir_parse_result = parse_args_and_redirection(cmd, line);
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
