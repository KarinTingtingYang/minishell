/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:26:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/19 09:51:19 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Add this new helper function at the top of src/executor/executor_helper.c
static int is_empty_or_whitespace(const char *str)
{
    if (!str)
        return (1);
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    return (*str == '\0');
}

/**
 * @brief Validates the segments of a command pipeline after splitting by '|'.
 * Checks for empty segments which are syntax errors.
 * @param parts The array of command strings.
 * @param count The number of command strings in the array.
 * @return 1 on successful validation, 0 on syntax error.
 */
static int validate_pipeline_parts(char **parts, int count)
{
    int i;

	if (count <= 1)
		return (1); // No pipes, or single command, no pipe syntax to check.

	i = 0;
	while (i < count)
	{
		if (is_empty_or_whitespace(parts[i]))
        {
            ft_error("", "syntax error near unexpected token `|'");
            return (0); // Validation failed.
        }
		i++;
	}
    return (1); // All parts are valid.
}
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
// int execute_prepared_command(t_execute_data *data)
// int execute_prepared_command(t_execute_data *data, t_process_data *process_data)
// {
// 	pid_t pid;
// 	int exit_code;
// 	int has_redirection = (data->input_file || data->output_file);

// 	if (is_builtin(data->clean_args[0]) && !has_redirection)
// 	{
// 		run_builtin(data->clean_args, data->env_list);
// 		free_execute_data(data);
// 		return (0);
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		// perror("fork"); // DEBUG: Print error if fork fails
// 		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
// 		free_execute_data(data);
// 		return (-1);
// 	}
// 	else if (pid == 0)
// 	{
// 		redirect_io(data->input_file, data->output_file, data->output_mode);
// 		if (is_builtin(data->clean_args[0]))
// 			run_builtin(data->clean_args, data->env_list);
// 		else
// 			execute_cmd(data->cmd_path, data->clean_args, data->path_dirs, data->env_list);
// 		exit(0);
// 	}
// 	else
// 	{
// 		exit_code = wait_for_child_and_handle_status(pid);
// 		//printf("Child process %d finished, returning to shell %d\n", pid, getpid());
// 		process_data->last_exit_status = exit_code; // Update the last exit status in process_data
// 		free_execute_data(data);
// 		return (exit_code);
// 	}
// }


int execute_prepared_command(t_execute_data *data, t_process_data *process_data)
{
    pid_t pid;
    int exit_code;
    int has_redirection = (data->input_file || data->output_file);

    if (is_builtin(data->clean_args[0]) && !has_redirection && !process_data->in_pipeline) // DEBUG
    {
        run_builtin(data->clean_args, data->env_list);
        free_execute_data(data);
        return (0);
    }

    pid = fork();
    if (pid < 0)
    {
        ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
        free_execute_data(data);
        return (-1);
    }
    else if (pid == 0)
    {
        redirect_io(data->input_file, data->output_file, data->output_mode);
        if (is_builtin(data->clean_args[0]))
            run_builtin(data->clean_args, data->env_list);
        else
            execute_cmd(data->cmd_path, data->clean_args, data->path_dirs, data->env_list);
        exit(0);
    }
    else
    {
        exit_code = wait_for_child_and_handle_status(pid);
        process_data->last_exit_status = exit_code;
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
int prepare_command_execution(char *line, t_env_var *env_list, t_execute_data *data, t_process_data *process_data)
{
	data->heredoc_file = NULL;
    if (line == NULL || *line == '\0')
	{
        return (0);		 // No command to execute
	}	
	data->original_args = tokenize_input(line); // DEBUG: tokenize_input() should handle splitting by spaces
    if (!data->original_args || !data->original_args[0])
    {
		// ft_putstr_fd("minishell: syntax error (unclosed quote)\n", STDERR_FILENO); // DEBUG: changed to a more generic error message
		ft_error("", "syntax error (unclosed quote)");
		free_split(data->original_args);
        // return (0);		 // no command to execute // but we still want to return 1 to indicate success
		return (1); // no command to execute, but we still want to return 1 to indicate success
    }
    data->clean_args = handle_redirection(data->original_args, &data->input_file, 
		&data->output_file, &data->output_mode, &data->heredoc_file, env_list, process_data->last_exit_status);
    if (!data->clean_args)
    {
		// DEBUG FIX: centralise freeing of resources in the caller function 
		// // Cleanup if handle_redirection fails
        // if (data->heredoc_file)
        // {
        //     unlink(data->heredoc_file);
        //     free(data->heredoc_file);
		// 	data->heredoc_file = NULL; // Ensure it's set to NULL after freeing
        // }
        // free_split(data->original_args);
        return (2); // error in redirection or syntax
    }
    if (!data->clean_args[0])
    {
        // free_split(data->original_args);
        // free(data->clean_args);
		// data->clean_args = NULL; // Ensure clean_args is set to NULL after freeing
        return (0);	 // no command to execute
    }
	if (is_builtin(data->clean_args[0]))
    {
        data->cmd_path = NULL;
        data->path_dirs = find_path_dirs(env_list);
        return (1); // Success
    }
    data->path_dirs = find_path_dirs(env_list);
    if (!data->path_dirs)
    {
		ft_error("", "PATH variable not found"); // DEBUG: Print error if PATH not found
        // printf("Error: PATH variable not found\n"); // DEBUG: Print error if PATH not found
        // free_split(data->original_args);
        // free_split(data->clean_args);
        return (1); // error in finding path directories
    }
    data->cmd_path = find_full_cmd_path(data->clean_args[0], data->path_dirs);
    if (!data->cmd_path)
    {
        // command not found
        // free_split(data->original_args);
        // free_split(data->clean_args);
        // free_split(data->path_dirs);
        return (127); // typical command not found exit code
    }
	data-> env_list= env_list; // Store the environment variables for built-in commands
    return (1); // success
}

/**
 * @brief Builds command structures from split command parts.
 * 
 * This function iterates through the split command parts, creates a `t_command`
 * structure for each part, and populates it with arguments and redirection
 * information. It uses the provided environment variables to resolve command paths.
 * If any step fails (e.g., memory allocation, command creation), it cleans up
 * previously allocated resources and returns `NULL`.
 * @param cmds     Pointer to an array of `t_command*` where commands will be stored.
 * @param parts    Array of command parts split by pipes.
 * @param index    Current index in the parts array.
 * @param count    Total number of command parts.
 * @param envp     The environment variable array used to resolve command paths.
 * @return Returns 1 on success, or 0 if an error occurs (e.g., memory allocation
 *         failure).
 */
// static int build_commands_from_parts(t_command **cmds, char **parts, int index, 
// 	int count, t_process_data *process_data)
// {
//     char **path_dirs;
//     // char **tokens;

//     if (index == 0)
//     {
//         path_dirs = find_path_dirs(process_data->env_list);
//         if (path_dirs == NULL)
//         {
//             // ft_putstr_fd("minishell: PATH not found\n", STDERR_FILENO); // DEBUG: Print error if PATH not found
// 			ft_error("", "PATH not found");
//             return (0);
//         }
//         while (index < count)
//         {
//             t_token **tokens = parse_line(parts[index]);
//             if (tokens == NULL || !validate_redirect_syntax(tokens))
// 			{
// 				// parse_line will have already printed an error message
// 				// ft_putstr_fd("minishell: syntax error (unclosed quote)\n", STDERR_FILENO);
// 				free_tokens(tokens);
// 				free_split(path_dirs);
// 				return (0);
// 			}
			
// 			// EXPANSION DEBUGGING: we need to call the expand_and_split_args function
// 			char **expanded_args = expand_and_split_args(tokens, process_data->env_list,
// 				process_data->last_exit_status);
// 			free_tokens(tokens);
// 			if (expanded_args == NULL)
// 			{
// 				free_split(path_dirs);
// 				return (0); // error in expanding or splitting args
// 			}
// 			// ======== END EXPANSION DEBUGGING ========
			
//             cmds[index] = create_command(expanded_args, path_dirs, process_data);
//             free_split(expanded_args);

//             if (cmds[index] == NULL)
// 			{
// 				// ft_putstr_fd("minishell: command creation failed\n", STDERR_FILENO); // DEBUG: Print error if command creation fails
// 				ft_error("", "command creation failed");
// 				free_split(path_dirs);
// 				return (0);
// 			}
// 			process_data[index].in_pipeline = (count > 1); // DEBUGGING
//             index++;
//         }
//         free_split(path_dirs);
//         return (1);
//     }
//     return (1);
// }

static int build_commands_from_parts(t_command **cmds, char **parts, int index, int count,
										t_process_data *process_data)
{
    char        **path_dirs;
    t_token     **tokens;
    int         i;

    if (index != 0)
        return (1);
    path_dirs = find_path_dirs(process_data->env_list);
    if (path_dirs == NULL)
    {
        ft_error(NULL, "PATH not found");
        return (0);
    }
    i = 0;
    while (i < count)
    {
        tokens = parse_line(parts[i]);
        if (tokens == NULL)
        {
            free_split(path_dirs);
            return (0);
        }
        if (!validate_redirect_syntax(tokens))
        {
            free_tokens(tokens);
            free_split(path_dirs);
            return (0);
        }
        free_tokens(tokens);
        i++;
    }
    while (index < count)
    {
        char    **expanded_args;

        tokens = parse_line(parts[index]);
        if (tokens == NULL)
        {
            free_split(path_dirs);
            return (0);
        }
        expanded_args = expand_and_split_args(tokens, process_data->env_list,
                                              process_data->last_exit_status);
        free_tokens(tokens);
        if (expanded_args == NULL)
        {
            free_split(path_dirs);
            return (0);
        }
        cmds[index] = create_command(expanded_args, path_dirs, &process_data[index]);
        free_split(expanded_args);

        if (cmds[index] == NULL)
        {
            ft_error(NULL, "command creation failed");
            free_split(path_dirs);
            return (0);
        }

        process_data[index].in_pipeline = (count > 1);
        index++;
    }

    free_split(path_dirs);
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
t_command **prepare_pipeline_commands(char *line, int *count, char ***parts, 
	t_process_data *process_data)
{
	t_command **cmds;
	
	*parts = split_line_by_pipe(line);
    if (*parts == NULL || (*parts)[0] == NULL)
    {
		// ft_putstr_fd("minishell: syntax error (unclosed quote)\n", STDERR_FILENO); // DEBUG: Print error if split fails
		ft_error("", "syntax error (unclosed quote)");
        if (*parts != NULL)
            free_split(*parts);
        return (NULL);
    }

	// ---> FIX: VALIDATE THE COMMAND PARTS <---
    *count = count_command_parts(*parts);
    if (!validate_pipeline_parts(*parts, *count))
    {
        free_split(*parts);
        return (NULL);
    }
	// If a single command is empty (e.g., user just hits enter), it's not a pipe error.
    if (*count == 1 && is_empty_or_whitespace((*parts)[0]))
    {
        free_split(*parts);
        return (NULL);
    }
	// ---> END OF FIX <---
	
	cmds = malloc(sizeof(t_command *) * (*count + 1));
    if (cmds == NULL)
	{
		// ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO); // DEBUG: Print error if malloc fails
		// free_split(*parts); // DEBUG: Free the parts array if malloc fails
		// return (NULL); // DEBUG: Return NULL if malloc fails
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	}
    ft_bzero(cmds, sizeof(t_command *) * (*count + 1));
    if (!build_commands_from_parts(cmds, *parts, 0, *count, process_data))
    {
        free_commands_recursive(cmds, 0, *count);
        free(cmds);
        free_split(*parts);
        return (NULL);
    }
	int i = 0;
	while (i < *count)
	{
		process_data[i].in_pipeline = (*count > 1);  // DEBUG
		i++;
	}
    return (cmds);
}

/**
 * @brief Duplicates an array of strings (e.g., command arguments).
 * 
 * This function allocates a new array of strings and copies each string
 * from the original array into the new one. It ensures that the new array
 * is NULL-terminated. If memory allocation fails, it cleans up and returns NULL.
 * @param args The original array of strings to duplicate.
 * @return A newly allocated array of strings, or NULL on failure.
 */
char **ft_split_dup(char **args)
{
	int		count = 0;
	char	**dup;
	int		i;

	if (args == NULL)
		return (NULL);
	while (args[count])
		count++;
	dup = malloc((count + 1) * sizeof(char *));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		dup[i] = ft_strdup(args[i]);
		if (dup[i] == NULL)
		{
			// If strdup fails, we need to free previously allocated strings
			while (--i >= 0)
				free(dup[i]);
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[count] = NULL;
	return (dup);
}
