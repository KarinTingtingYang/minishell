/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor_helper.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: Invalid date        by               #+#    #+#                 */
/*   Updated: 2025/09/03 14:55:51 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is NULL, empty, or contains only whitespace.
 * 
 * This function iterates through the string and verifies if all characters
 * are spaces or tabs. If the string is NULL or empty, it also returns true.
 * 
 * @param str The string to check.
 * @return 1 if the string is NULL, empty, or only whitespace; 0 otherwise.
 */
int is_empty_or_whitespace(const char *str)
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
int validate_pipeline_parts(char **parts, int count)
{
    int i;

	if (count <= 1)
		return (1);
	i = 0;
	while (i < count)
	{
		if (is_empty_or_whitespace(parts[i]))
        {
            ft_error("", "syntax error near unexpected token `|'");
            return (0);
        }
		i++;
	}
    return (1);
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

/**
 * @brief Counts the number of command parts in an array of strings.
 * 
 * This function iterates through the array of strings until it finds a NULL
 * terminator, counting the number of valid command parts.
 * @param parts The array of command strings.
 * @return The count of command parts in the array.
 */
int count_command_parts(char **parts)
{
    int count = 0;
    while (parts[count] != NULL)
        count++;
    return (count);
}

int	is_unquoted_pipe_present(const char *line)
{
	char	quote_char;

	quote_char = 0;
	while (*line)
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (*line == quote_char)
			quote_char = 0;
		else if (quote_char == 0 && *line == '|')
			return (1);
		line++;
	}
	return (0);
}

// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------
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


// int execute_prepared_command(t_execute_data *data, t_process_data *process_data)
// {
//     pid_t pid;
//     int exit_code;
//     int has_redirection = (data->input_file || data->output_file);

//     if (is_builtin(data->clean_args[0]) && !has_redirection && !process_data->in_pipeline) // DEBUG
//     {
//         run_builtin(data->clean_args, process_data);
//         free_execute_data(data);
//         return (0);
//     }

// 	// EXIT CODE DEBUG
// 	 // Ignore SIGINT in the parent shell before forking
//     signal(SIGINT, SIG_IGN);

//     pid = fork();
//     if (pid < 0)
//     {
//         ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
//         free_execute_data(data);
//         return (-1);
//     }
//     else if (pid == 0)
//     {
//         redirect_io(data->input_file, data->output_file, data->output_mode);
//         if (is_builtin(data->clean_args[0]))
//             run_builtin(data->clean_args, process_data);
//         else
//             execute_cmd(data->cmd_path, data->clean_args, data->path_dirs, data->env_list);
//         exit(0);
//     }
//     else
//     {
//         exit_code = wait_for_child_and_handle_status(pid);
// 		// EXIT CODE DEBUG
// 		 // Restore the parent's signal handlers
//         setup_signal_handlers();
//         process_data->last_exit_status = exit_code;
//         free_execute_data(data);
//         return (exit_code);
//     }
// }

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
// int prepare_command_execution(char *line, t_env_var *env_list, t_execute_data *data, t_process_data *process_data)
// {
// 	data->heredoc_file = NULL;
//     if (line == NULL || *line == '\0')
//         return (0);		 // No command to execute
// 	data->original_args = tokenize_input(line); // DEBUG: tokenize_input() should handle splitting by spaces
//     if (!data->original_args || !data->original_args[0])
//     {
// 		ft_error("", "syntax error (unclosed quote)");
// 		free_split(data->original_args);
// 		return (1); // no command to execute, but we still want to return 1 to indicate success
//     }
//     data->clean_args = handle_redirection(data->original_args, process_data, &data->input_file, 
// 		&data->output_file, &data->output_mode, &data->heredoc_file);
//     if (!data->clean_args)
//         return (2); // error in redirection or syntax
//     if (!data->clean_args[0])
//         return (0);	 // no command to execute
// 	if (is_builtin(data->clean_args[0]))
//     {
//         data->cmd_path = NULL;
//         data->path_dirs = find_path_dirs(env_list);
//         return (1); // Success
//     }
//     data->path_dirs = find_path_dirs(env_list);
//     if (!data->path_dirs)
//     {
// 		ft_error("", "PATH variable not found"); // DEBUG: Print error if PATH not found
//         return (1); // error in finding path directories
//     }
//     data->cmd_path = find_full_cmd_path(data->clean_args[0], data->path_dirs);
//     if (!data->cmd_path)
//         return (127); // typical command not found exit code
// 	data-> env_list= env_list; // Store the environment variables for built-in commands
//     return (1); // success
// }

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


// int build_commands_from_parts(t_command **cmds, char **parts, int index, int count,
//                               t_process_data *process_data)
// {
//     char      **path_dirs;
//     t_token   **tokens;
//     char      **expanded_args;

//     path_dirs = find_path_dirs(process_data->env_list);
//     if (path_dirs == NULL)
//     {
//         ft_error(NULL, "PATH variable not found");
//         return 0;
//     }

//     process_data->in_pipeline = (count > 1);

//     while (index < count)
//     {
//         tokens = parse_line(parts[index]);
//         if (tokens == NULL)
//         {
//             free_split(path_dirs);
//             return 0;
//         }

//         expanded_args = expand_and_split_args(tokens,
//                                               process_data->env_list,
//                                               process_data->last_exit_status);
//         free_tokens(tokens);
//         if (expanded_args == NULL)
//         {
//             free_split(path_dirs);
//             return 0;
//         }

//         /*real arg instead of comment; same process_data pointer */
//         cmds[index] = create_command(expanded_args, path_dirs, process_data);
//         free_split(expanded_args);
//         if (cmds[index] == NULL)
//         {
// 			 // If creation failed due to Ctrl+C in a heredoc, fail silently.
//             if (g_signal_received == SIGINT)
//             {
//                 free_split(path_dirs);
//                 return (0);
//             }
//             ft_error(NULL, "command creation failed");
//             free_split(path_dirs);
//             return 0;
//         }
//         index++;
//     }

//     free_split(path_dirs);
//     return 1;
// }

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
// t_command **prepare_pipeline_commands(char *line, int *count, char ***parts, 
// 	t_process_data *process_data)
// {
// 	t_command **cmds;
	
// 	*parts = split_line_by_pipe(line);
//     if (*parts == NULL || (*parts)[0] == NULL)
//     {
// 		// ft_putstr_fd("minishell: syntax error (unclosed quote)\n", STDERR_FILENO); // DEBUG: Print error if split fails
// 		ft_error("", "syntax error (unclosed quote)");
//         if (*parts != NULL)
//             free_split(*parts);
//         return (NULL);
//     }

// 	// ---> FIX: VALIDATE THE COMMAND PARTS <---
//     *count = count_command_parts(*parts);
//     if (!validate_pipeline_parts(*parts, *count))
//     {
//         free_split(*parts);
//         return (NULL);
//     }
// 	// If a single command is empty (e.g., user just hits enter), it's not a pipe error.
//     if (*count == 1 && is_empty_or_whitespace((*parts)[0]))
//     {
//         free_split(*parts);
//         return (NULL);
//     }
// 	// ---> END OF FIX <---
	
// 	cmds = malloc(sizeof(t_command *) * (*count + 1));
//     if (cmds == NULL)
// 	{
// 		// ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO); // DEBUG: Print error if malloc fails
// 		// free_split(*parts); // DEBUG: Free the parts array if malloc fails
// 		// return (NULL); // DEBUG: Return NULL if malloc fails
// 		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
// 	}
//     ft_bzero(cmds, sizeof(t_command *) * (*count + 1));
//     if (!build_commands_from_parts(cmds, *parts, 0, *count, process_data))
//     {
//         free_commands_recursive(cmds, 0, *count);
//         free(cmds);
//         free_split(*parts);
//         return (NULL);
//     }
// 	int i = 0;
// 	while (i < *count)
// 	{
// 		process_data[i].in_pipeline = (*count > 1);  // DEBUG
// 		i++;
// 	}
//     return (cmds);
// }

// t_command **prepare_pipeline_commands(char *line, int *count, char ***parts,
//                                       t_process_data *process_data)
// {
//     t_command **cmds;
//     char      **path_dirs;
//     int         i;

//     *parts = split_line_by_pipe(line);
//     if (*parts == NULL || (*parts)[0] == NULL)
//         return (NULL);

//     *count = count_command_parts(*parts);
//     if (!validate_pipeline_parts(*parts, *count))
//     {
//         free_split(*parts);
//         *parts = NULL;
//         return (NULL);
//     }

//     cmds = malloc(sizeof(t_command *) * ((size_t)(*count) + 1));
//     if (cmds == NULL)
//     {
//         free_split(*parts);
//         *parts = NULL;
//         return (NULL);
//     }
//     ft_bzero(cmds, sizeof(t_command *) * ((size_t)(*count) + 1));

//     /* set once for the whole pipeline */
//     if (process_data)
//         process_data->in_pipeline = (*count > 1);

//     /* get PATH dirs once for the whole pipeline */
//     path_dirs = find_path_dirs(process_data->env_list);
//     if (path_dirs == NULL)
//     {
//         ft_error(NULL, "PATH variable not found");
//         free(cmds);
//         free_split(*parts);
//         *parts = NULL;
//         return (NULL);
//     }

//     /* build each command with the SAME process_data pointer */
//     i = 0;
//     while (i < *count)
//     {
//         t_token **tokens = parse_line((*parts)[i]);
//         char    **expanded;

//         if (tokens == NULL)
//         {
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }

//         expanded = expand_and_split_args(tokens,
//                                          process_data->env_list,
//                                          process_data->last_exit_status);
//         free_tokens(tokens);
//         if (expanded == NULL)
//         {
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }

//         /* real call: pass path_dirs and the SAME process_data (no indexing) */
//         cmds[i] = create_command(expanded, path_dirs, process_data);
//         free_split(expanded);
//         if (cmds[i] == NULL)
//         {
// 			 // If creation failed but a signal was received, do not print an error.
//             if (g_signal_received != SIGINT)
//                 ft_error(NULL, "command creation failed");
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }
//         i++;
//     }

//     free_split(path_dirs);      /* path_dirs no longer needed */
//     cmds[*count] = NULL;
//     return cmds;
// }

// t_command **prepare_pipeline_commands(char *line, int *count, char ***parts,
//                                       t_process_data *process_data)
// {
//     t_command **cmds;
//     char      **path_dirs;
//     int         i;

// 	/* ---- PRECHECK on raw line: outside quotes, detect bad redirections ---- */
// 	if (line)
// 	{
// 		int  i;
// 		char q;

// 		i = 0;
// 		q = 0;
// 		while (line[i] != '\0')
// 		{
// 			/* enter/exit quotes */
// 			if ((line[i] == '\'' || line[i] == '"') && q == 0)
// 			{
// 				q = line[i];
// 				i++;
// 				continue;
// 			}
// 			if (q && line[i] == q)
// 			{
// 				q = 0;
// 				i++;
// 				continue;
// 			}

// 			if (q == 0 && (line[i] == '<' || line[i] == '>'))
// 			{
// 				int  op_len;
// 				int  j;

// 				/* operator length: <<<, <>, <<, >>, <, > */
// 				op_len = 1;
// 				if (line[i] == '<')
// 				{
// 					if (line[i + 1] == '<' && line[i + 2] == '<')
// 						op_len = 3;              /* <<< */
// 					else if (line[i + 1] == '>')
// 						op_len = 2;              /* <>  */
// 					else if (line[i + 1] == '<')
// 						op_len = 2;              /* <<  */
// 				}
// 				else /* '>' */
// 				{
// 					if (line[i + 1] == '>')
// 						op_len = 2;              /* >>  */
// 				}

// 				j = i + op_len;

// 				/* skip spaces after operator */
// 				while (line[j] == ' ' || line[j] == '\t')
// 					j++;

// 				/* choose bash-like token to report */
// 				if (line[j] == '\0')
// 				{
// 					ft_error(NULL, "syntax error near unexpected token `newline'");
// 					if (process_data) process_data->last_exit_status = 2;
// 					return NULL; /* abort early */
// 				}
// 				if (line[j] == '|')
// 				{
// 					ft_error(NULL, "syntax error near unexpected token `|'");
// 					if (process_data) process_data->last_exit_status = 2;
// 					return NULL;
// 				}
// 				if (line[j] == '<')
// 				{
// 					/* Special case: bash-3.2 prints `newline' for `<<<' */
// 					if (op_len == 3) {
// 						ft_error(NULL, "syntax error near unexpected token `newline'");
// 						if (process_data) process_data->last_exit_status = 2;
// 						return NULL;
// 					}
// 					ft_error(NULL, "syntax error near unexpected token `<'");
// 					if (process_data) process_data->last_exit_status = 2;
// 					return NULL;
// 				}
// 				if (line[j] == '>')
// 				{
// 					/* For `<>>` bash prints just `>' */
// 					ft_error(NULL, "syntax error near unexpected token `>'");
// 					if (process_data) process_data->last_exit_status = 2;
// 					return NULL;
// 				}

// 				/* ok, continue scanning */
// 				i = j;
// 				continue;
// 			}

// 			i++;
// 		}
// 	}
// 	/* ---- END PRECHECK ---- */


//     *parts = split_line_by_pipe(line);
//     if (*parts == NULL || (*parts)[0] == NULL)
//         return (NULL);

//     *count = count_command_parts(*parts);

//     /* optional: reset if you have this field; ignore if not */
//     if (process_data)
//         process_data->syntax_error = 0;

//     if (!validate_pipeline_parts(*parts, *count))
//     {
//         free_split(*parts);
//         *parts = NULL;
//         return (NULL);
//     }

//     cmds = malloc(sizeof(t_command *) * ((size_t)(*count) + 1));
//     if (cmds == NULL)
//     {
//         free_split(*parts);
//         *parts = NULL;
//         return (NULL);
//     }
//     ft_bzero(cmds, sizeof(t_command *) * ((size_t)(*count) + 1));

//     /* set once for the whole pipeline */
//     if (process_data)
//         process_data->in_pipeline = (*count > 1);

//     /* get PATH dirs once for the whole pipeline */
//     path_dirs = find_path_dirs(process_data->env_list);
//     // if (path_dirs == NULL)
//     // {
//     //     ft_error(NULL, "PATH variable not found");
//     //     free(cmds);
//     //     free_split(*parts);
//     //     *parts = NULL;
//     //     return (NULL);
//     // }

//     /* build each command with the SAME process_data pointer */
//     i = 0;
//     while (i < *count)
//     {
//         t_token **tokens = parse_line((*parts)[i]);
//         char    **expanded;

//         if (tokens == NULL)
//         {
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }

//         expanded = expand_and_split_args(tokens,
//                                          process_data->env_list,
//                                          process_data->last_exit_status);
//         free_tokens(tokens);
//         if (expanded == NULL)
//         {
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }

//         /* real call: pass path_dirs and the SAME process_data (no indexing) */
//         cmds[i] = create_command(expanded, path_dirs, process_data);
//         free_split(expanded);
//         if (cmds[i] == NULL)
//         {
//             /* IMPORTANT: do NOT print here; a syntax error (if any) was already printed upstream */
//             free_split(path_dirs);
//             free_commands_recursive(cmds, 0, i);
//             free(cmds);
//             free_split(*parts);
//             *parts = NULL;
//             return (NULL);
//         }
//         i++;
//     }

//     // free_split(path_dirs);      /* path_dirs no longer needed */
// 	if (path_dirs)
//     	free_split(path_dirs);
//     cmds[*count] = NULL;
//     return cmds;
// }
