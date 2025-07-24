/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:26:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 09:59:05 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Executes a previously prepared command in a child process.
 * 
 * This function forks the current process and runs the command described
 * by the provided `t_execute_data` structure in the child process. It handles:
 * - Applying input/output redirection.
 * - Executing the command via `execve()`.
 * - Waiting for the child process to complete and capturing its exit status.
 * - Freeing all dynamically allocated memory stored in `t_execute_data`.
 * @param data Pointer to a filled `t_execute_data` struct containing all
 *             necessary information for execution (arguments, redirection, etc.).
 * @return Returns:
 *         - The child process's exit code on success.
 *         - -1 if `fork()` fails.
 */
int execute_prepared_command(t_execute_data *data)
{
    pid_t pid;
    int exit_code;

    pid = fork();
    if (pid < 0)
    {
		free_execute_data(data);
        return (-1);
    }
    else if (pid == 0)
    {
        redirect_io(data->input_file, data->output_file);
        execute_cmd(data->cmd_path, data->clean_args, data->path_dirs);
        exit(127);
    }
    else
    {
        exit_code = wait_for_child_and_handle_status(pid);
        free_execute_data(data);
        return (exit_code);
    }
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
 * If any of these steps fail, it cleans up partially allocated resources and returns
 * a specific code to indicate the error or status.
 * @param line The command line input to be processed.
 * @param envp The environment variables (used to extract PATH).
 * @param data Pointer to `t_execute_data` where parsed and prepared data is stored.
 * @return Returns:
 *         - 0 if the command is empty or invalid.
 *         - 1 on successful preparation.
 *         - 2 if there is a redirection syntax or file error.
 *         - 127 if the command was not found in PATH.
 */
int prepare_command_execution(char *line, char **envp, t_execute_data *data)
{
    if (line == NULL || *line == '\0')
        return (0);		 // No command to execute
    data->original_args = ft_split(line, ' ');
    if (!data->original_args || !data->original_args[0])
    {
        free_split(data->original_args);
        return (0);		 // no command to execute
    }
    data->clean_args = handle_redirection(data->original_args, &data->input_file, &data->output_file);
    if (!data->clean_args)
    {
        free_split(data->original_args);
        return (2); // error in redirection or syntax
    }
    if (!data->clean_args[0])
    {
        free_split(data->original_args);
        free(data->clean_args);
        return (0);	 // no command to execute
    }
    data->path_dirs = find_path_dirs(envp);
    if (!data->path_dirs)
    {
        printf("Error: PATH variable not found\n");
        free_split(data->original_args);
        free_split(data->clean_args);
        return (1); // error in finding path directories
    }
    data->cmd_path = find_full_cmd_path(data->clean_args[0], data->path_dirs);
    if (!data->cmd_path)
    {
        // command not found
        free_split(data->original_args);
        free_split(data->clean_args);
        free_split(data->path_dirs);
        return (127); // typical command not found exit code
    }
    return (1); // success
}

/**
 * @brief Builds an array of command structures from split pipeline parts.
 * 
 * Allocates and fills `t_command` structures for each part of a piped command.
 * Only processes the command parts if the starting index is 0 (initial call),
 * otherwise returns success immediately (useful for recursion-like interface).
 * The function resolves the command paths using the PATH environment variable.
 * If any command fails to be created or if PATH is missing, cleanup is performed
 * and the function returns failure.
 * @param cmds     The array of `t_command*` to populate.
 * @param parts    The array of strings representing individual piped commands.
 * @param index    The current index (usually 0) to start from.
 * @param count    The total number of command parts to process.
 * @param envp     The environment variables used to resolve PATH.
 * 
 * @return Returns 1 on success, or 0 on failure (e.g., PATH not found or malloc failure).
 */
static int build_commands_from_parts(t_command **cmds, char **parts, int index, int count, char **envp)
{
	char **path_dirs;
    if (index == 0)
    {
        path_dirs = find_path_dirs(envp);
        if (path_dirs == NULL)
        {
            ft_putstr_fd("minishell: PATH not found\n", STDERR_FILENO);
            return (0);
        }
        while (index < count)
        {
            cmds[index] = create_command(parts[index], path_dirs);
            if (!cmds[index])
            {
                free_split(path_dirs);
                return (0);
            }
            index++;
        }
        free_split(path_dirs);
        return (1);
    }
    return (1);
}

/**
 * @brief Parses a piped command line and prepares an array of command structures.
 * 
 * This function splits the given line by the pipe symbol `'|'`, counts the resulting
 * command segments, allocates an array of `t_command*`, and builds command structures 
 * using the environment. If any step fails (e.g. syntax error, memory allocation, 
 * or command creation), it cleans up and returns `NULL`.
 * @param line     The full command line input containing pipes (e.g., "ls | grep .c").
 * @param count    Pointer to an integer where the number of parsed commands will
 *                 be stored.
 * @param parts    Pointer to a string array that will hold the split command parts.
 * @param envp     The environment variable array used to resolve command paths.
 * @return An allocated array of `t_command*` pointers ready for execution,
 *         or `NULL` on failure (syntax error, allocation failure, etc.).
 */
t_command **prepare_pipeline_commands(char *line, int *count, char ***parts, char **envp)
{
	t_command **cmds;
	
    *parts = ft_split(line, '|');
    if (*parts == NULL || (*parts)[0] == NULL)
    {
        ft_putstr_fd("Error: invalid pipe syntax\n", STDERR_FILENO);
        if (*parts != NULL)
            free_split(*parts);
        return (NULL);
    }
    *count = count_command_parts(*parts);
	cmds = malloc(sizeof(t_command *) * (*count + 1));
    if (cmds == NULL)
	{
		ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
		free_split(*parts);
		return (NULL);
	}
    cmds[*count] = NULL;
    if (!build_commands_from_parts(cmds, *parts, 0, *count, envp))
    {
        free_commands_recursive(cmds, 0, *count);
        free(cmds);
        free_split(*parts);
        return (NULL);
    }
    return (cmds);
}
