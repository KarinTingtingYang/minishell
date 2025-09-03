/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_pipeline_helper.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 11:30:14 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 14:31:27 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Sets up a single command in the process data structure.
 *
 * This function creates a command using the expanded arguments and
 * stores it in the process data at the specified index. It also frees
 * the expanded arguments array after use.
 *
 * @param expanded_args The array of expanded arguments for the command.
 * @param data Pointer to the process data structure.
 * @param index The index at which to store the created command.
 * @return 1 on success, 0 on failure.
 */
static int setup_command(char **expanded_args, t_process_data *data, int index)
{
    data->cmds[index] = create_command(expanded_args, data->path_dirs, data);
    free_split(expanded_args);
    if (data->cmds[index] == NULL)
    {
        if (g_signal_received == SIGINT)
            return (0);
        ft_error(NULL, "command creation failed");
        return (0);
    }
    return (1);
}

/**
 * @brief Parses and expands the command arguments.
 *
 * This function takes a command string, parses it into tokens,
 * expands the tokens using environment variables, and returns
 * the resulting array of expanded arguments.
 *
 * @param cmd_str The command string to parse and expand.
 * @param data Pointer to the process data structure for environment context.
 * @return An array of expanded arguments, or NULL on failure.
 */
static char **expand_command_args(char *cmd_str, t_process_data *data)
{
    t_token **tokens = parse_line(cmd_str);
    char **expanded_args;

    if (tokens == NULL)
        return (NULL);
    expanded_args = expand_and_split_args(tokens, data->env_list, 
                                        data->last_exit_status);
    free_tokens(tokens);
    return (expanded_args);
}

/**
 * @brief Builds a single command from the command parts.
 *
 * This function expands the command arguments and sets up
 * the command in the process data structure at the specified index.
 *
 * @param parts Array of command parts (strings).
 * @param data Pointer to the process data structure.
 * @param index The index of the command part to process.
 * @return 1 on success, 0 on failure.
 */
static int build_single_command(char **parts, t_process_data *data, int index)
{
    char **expanded_args;

    expanded_args = expand_command_args(parts[index], data);
    if (expanded_args == NULL)
        return (0);
    return (setup_command(expanded_args, data, index));
}

/**
 * @brief Initializes the pipeline build process.
 *
 * This function retrieves the PATH directories from the environment
 * list and stores them in the process data structure. It returns
 * 1 on success or 0 if the PATH variable is not found.
 *
 * @param data Pointer to the process data structure.
 * @return 1 on success, 0 on failure.
 */
static int initialize_pipeline_build(t_process_data *data)
{
    data->path_dirs = find_path_dirs(data->env_list);
    if (data->path_dirs == NULL)
    {
        ft_error(NULL, "PATH variable not found");
        return (0);
    }
    return (1);
}

/**
 * @brief Builds command structures from split command parts.
 *
 * This function initializes the pipeline build process, iterates
 * over each command part, expands its arguments, and sets up
 * the corresponding command in the process data structure.
 *
 * @param parts Array of command parts split by pipes.
 * @param count Total number of command parts.
 * @param data Pointer to the process data structure.
 * @return 1 on success, 0 on failure.
 */
int build_commands_from_parts(char **parts, int count, t_process_data *data)
{
    int index;

    if (!initialize_pipeline_build(data))
        return (0);
    data->in_pipeline = (count > 1);
    index = 0;
    while (index < count)
    {
        if (!build_single_command(parts, data, index))
        {
            free_split(data->path_dirs);
            return (0);
        }
        index++;
    }
    free_split(data->path_dirs);
    return (1);
}
