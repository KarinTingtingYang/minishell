/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/05 10:57:58 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char **environ;
extern volatile sig_atomic_t g_child_running; // Declare extern for global flag

/**
 * @brief Handles errors that occur during the `execve` system call.
 *
 * This function is called when `execve` fails in a child process.
 * It determines the appropriate exit status based on the `errno` value:
 * - If the command is not found (`ENOENT`), it prints a
 * "command not found" message and exits with 127.
 * - For other errors, it uses `perror` to print the system error
 * and exits with status 1.
 * It also frees the allocated `cmd_path` before exiting.
 * @param cmd_path    The command path that failed to execute. This will be freed.
 * @param args        Command arguments (unused, but accepted for consistency).
 * @param path_dirs   PATH directories array (unused in this function).
 *
 * @exit 127 if the command is not found (`ENOENT`), otherwise exits with 1.
 */
static void handle_execve_error(char *cmd_path, char **args, char **path_dirs)
{
	int exit_status;
	(void)args;
	(void)path_dirs;

	if (errno == ENOENT)
	{
		write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit_status = 127;
	}
	else
	{
		perror(cmd_path);
		exit_status = 1;
	}
	free(cmd_path);
	exit(exit_status);
}

/**
 * @brief Executes a command using `execve` after resetting signal handlers.
 *
 * This function is intended to be run in the child process after a `fork()`.
 * It attempts to execute the given command using the full path. If the command
 * is not found or `execve` fails, it prints an appropriate error message and exits.
 * The signal handlers are reset to their default behavior before executing
 * the command to avoid inherited custom handlers from the shell.
 * @param cmd_path   Full path to the executable command. If NULL, an error is shown.
 * @param args       Null-terminated array of arguments, where args[0] is the
 *                   command name.
 * @param path_dirs  Array of PATH directories used for resolving the command
 *                   (freed on error).
 * @note This function does not return if successful. On failure, it exits the
 * child process. @exit 127 if the command is not found.
 */
void execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list)
{
    // // Reset signal handlers to default for the child process
    // reset_child_signal_handlers(); //
	// if (cmd_path == NULL)
	// {
	// 	free_split(path_dirs);
	// 	ft_putstr_fd(args[0], STDERR_FILENO);
	// 	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	// 	exit(127);
	// }
	// execve(cmd_path, args, environ);
	// handle_execve_error(cmd_path, args, path_dirs);

	// char **path_dirs = find_path_dirs(env_list);
	// if (!path_dirs)
	// {
	// 	ft_putendl_fd("Error: PATH variable not found", 2);
	// 	return (1); // Don't continue execution
	// }

	// char *cmd_path = find_full_path(path_dirs, args[0]);
	// if (!cmd_path)
	// {
	// 	ft_putendl_fd("Command not found", 2);
	// 	free_split(path_dirs);
	// 	return (127);
	// }

	// execve(cmd_path, args, convert_env_list_to_array(env_list));

	// // If execve fails:
	// perror("execve failed");
	// free(cmd_path);
	// free_split(path_dirs);
	// exit(1);

	// DEBUG FIX EXPORT :)
	char **envp;

    // Reset signal handlers to their default behavior for the child process.
    reset_child_signal_handlers();

    // Convert our linked list of environment variables into a char** array.
    envp = env_list_to_array(env_list); // Using the helper from utils.c

    // The command path should already be validated, but we check again.
    if (cmd_path == NULL)
    {
        if (args && args[0])
        {
             ft_putstr_fd("minishell: ", STDERR_FILENO);
             ft_putstr_fd(args[0], STDERR_FILENO);
             ft_putstr_fd(": command not found\n", STDERR_FILENO);
        }
        if (envp)
            free_split(envp);
        exit(127);
    }

    // Execute the command. This line should not return if it succeeds.
    execve(cmd_path, args, envp);

    // If execve *does* return, it means an error occurred.
    // We must clean up and exit the child process with an error code.
    free_split(envp);
    handle_execve_error(cmd_path, args, path_dirs); // This function will exit.
}

/**
 * @brief Executes a single shell command with optional redirections.
 *
 * Prepares and executes a standalone command without pipes. This includes:
 * - Splitting the input line into arguments,
 * - Handling I/O redirection (e.g., `<`, `>`),
 * - Resolving the command path using the environment `PATH`,
 * - Forking a new process to run the command.
 * Delegates preparation to `prepare_command_execution()` and execution to 
 * `execute_prepared_command()`. Automatically handles cleanup of
 * allocated resources.
 * @param line  The raw command line input string.
 * @param envp  The environment variables passed to the shell.
 * @return Returns:
 *         - 0 if the line is empty or has only redirections,
 *         - 1+ for successful execution of the command,
 *         - 2 or higher for syntax errors or other issues,
 *         - -1 on critical fork or execution errors.
 */
// static int execute_single_command(char *line, t_env_var *env_list)
// {
//     t_execute_data	data;
//     int				prepare_status;
// 	int				original_stdin;
// 	int				original_stdout;
// 	int				exit_status;

// 	  // KEY CHANGE HERE: Zero out the struct before use.
//     // This ensures all pointers inside are initialized to NULL.
//     ft_bzero(&data, sizeof(t_execute_data));

//     prepare_status = prepare_command_execution(line, env_list, &data);
//     if (prepare_status != 1)
// 	{
// 		free_execute_data(&data); // added to ensure cleanup
//         return (prepare_status); // 0 means empty, errors propagated
// 	}
	
// 	expand_args(data.clean_args, env_list, g_last_exit_status);
	
// 	if (!is_builtin(data.clean_args[0]))
// 		return (execute_prepared_command(&data));
// 	original_stdin = dup(STDIN_FILENO);
// 	original_stdout = dup(STDOUT_FILENO);
// 	if (apply_builtin_redirection(data.input_file, data.output_file, data.output_mode) == -1)
// 		exit_status = 1;
// 	else
// 		exit_status = run_builtin(data.clean_args, env_list);
// 	// Restore STDIN and STDOUT
// 	dup2(original_stdin, STDIN_FILENO);
// 	dup2(original_stdout, STDOUT_FILENO);
// 	close(original_stdin);
// 	close(original_stdout);
// 	free_execute_data(&data);
// 	return (exit_status);
// }


// static int execute_single_command(char *line, t_env_var *env_list)
static int execute_single_command(char *line, t_env_var *env_list, t_process_data *process_data)
{
    t_execute_data	data;
    int				prepare_status;
	int				original_stdin;
	int				original_stdout;
	int				exit_status;

	ft_bzero(&data, sizeof(t_execute_data));

	prepare_status = prepare_command_execution(line, env_list, &data);
	if (prepare_status != 1)
	{
		free_execute_data(&data);
		return (prepare_status);
	}

	char **expanded_args;
	// expand_args(data.clean_args, env_list, g_last_exit_status);
	// expand_args(data.clean_args, process_data->env_list, process_data->last_exit_status);
	expanded_args = expand_and_split_args(data.clean_args, process_data->env_list, process_data->last_exit_status);
	if (!expanded_args)
	{
		free_execute_data(&data);
		return (1);
	}

	free_split(data.clean_args);
	data.clean_args = expanded_args;

	if (data.clean_args[0] && !is_builtin(data.clean_args[0]))
	{
		// exit_status = execute_prepared_command(&data);
		exit_status = execute_prepared_command(&data, process_data);

		// Update global exit status here after external command
		// g_last_exit_status = exit_status;
		process_data->last_exit_status = exit_status;
		free_execute_data(&data);
		return exit_status;
	}

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);

	if (apply_builtin_redirection(data.input_file, data.output_file, data.output_mode) == -1)
		exit_status = 1;
	else
		exit_status = run_builtin(data.clean_args, env_list);

	// Restore STDIN and STDOUT
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);

	// Update global exit status after builtin command
	// g_last_exit_status = exit_status;
	process_data->last_exit_status = exit_status;
	free_execute_data(&data);

	return exit_status;
}


/**
 * @brief Prepares and executes a pipeline of shell commands.
 *
 * Splits the input command line by the pipe ('|') character, parses each segment
 * into a `t_command` structure, and sets up the necessary environment and file 
 * descriptors for inter-process communication using pipes.
 * It then executes the pipeline via `run_command_pipeline()` and ensures all 
 * dynamically allocated memory is properly freed afterward.
 * @param line  The input command line containing piped commands.
 * @param envp  The environment variables passed to the shell.
 * @return Returns the exit status of the final command in the pipeline:
 *         - 0 on success,
 *         - non-zero value on error or failure during preparation or execution.
 */
static int prepare_and_run_pipeline(char *line,  t_env_var *env_list)
{
    char **parts;
	t_command **cmds;
	char **path_dirs;
	int status;
    int count = 0;

	parts = NULL;
	count = 0;			
    cmds = prepare_pipeline_commands(line, &count, &parts, env_list);
    if (cmds == NULL)
        return (1);
    path_dirs = find_path_dirs(env_list);
	if (!path_dirs)
	{
		ft_putendl_fd("Error: PATH variable not found", 2);
		return (1);
	}
    status = run_command_pipeline(cmds, count, path_dirs, env_list);
    free_split(path_dirs);
    free_commands_recursive(cmds, 0, count);
    free(cmds);
    free_split(parts);
    return (status);
}

/**
 * @brief Executes a shell command, supporting both single and piped commands.
 *
 * This function checks whether the input line contains a pipe ('|') character.
 * If it does, the command is treated as a pipeline and processed accordingly
 * using `prepare_and_run_pipeline()`. Otherwise, it executes a single command
 * using `execute_single_command()`.
 * @param line The input command line entered by the user.
 * @param envp The environment variables passed to the shell.
 * @return Returns the exit status of the command:
 *         - 0 on success,
 *         - non-zero value on failure or command error.
 */
// int execute_command(char *line, t_env_var *env_list)
int execute_command(char *line, t_env_var *env_list, t_process_data *process_data)
{
    if (ft_strchr(line, '|'))
        return prepare_and_run_pipeline(line, env_list);
    else
        return execute_single_command(line, env_list, process_data);
}
