/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/22 10:08:55 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/22 14:30:54 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Recursively waits for all child processes in a pipeline.
 *
 * This function now tracks whether a signal message has been printed to avoid
 * duplicate messages (e.g., multiple newlines). It will print a message for
 * the first process it finds that was terminated by a signal.
 * @param pids            Array of process IDs to wait for.
 * @param index           Current index in the PID array.
 * @param max             Total number of processes.
 * @param last_status     Last known exit status (updated on the last child).
 * @param signal_printed  Pointer to a flag indicating if a signal message has been printed.
 * @return The exit status of the last process.
 */
static int wait_all_children(pid_t *pids, int index, int max, int last_status, int *signal_printed)
{
    int status;

    if (index >= max)
        return (last_status);
    if (waitpid(pids[index], &status, 0) == -1)
    {
        // perror("waitpid"); // DEBUG: Print error if waitpid fails
		ft_error_and_exit("waitpid", strerror(errno), EXIT_FAILURE);
        return (wait_all_children(pids, index + 1, max, -1, signal_printed));
    }
    if (WIFEXITED(status))
    {
        if (index == max - 1)
            last_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        // if (index == max - 1)
        // {
		// 	print_signal_message(status);
		// 	last_status = 128 + WTERMSIG(status);
		// }
		// EXIT CODE DEBUG
		if (!(*signal_printed))
		{
			print_signal_message(status);
			*signal_printed = 1;
		}
        if (index == max - 1)
			last_status = 128 + WTERMSIG(status);
    }
    return (wait_all_children(pids, index + 1, max, last_status, signal_printed));
}

/**
 * @brief Recursively forks child processes for each command in a pipeline.
 *
 * For each command in the pipeline, this function:
 * - Forks a child process.
 * - In the child:
 *   - Resets signal handlers.
 *   - Redirects input/output if specified or based on pipe position.
 *   - Closes unused pipe ends.
 *   - Executes the command using `execve`.
 * If a fork fails, the function prints an error and returns -1.
 * If execution continues in the parent, it recurses to the next command.
 * @param data Pointer to a t_process_data struct containing all pipeline data.
 * @param i The current index of the command being forked.
 * @return 0 on success, -1 on fork failure.
 */
static int fork_all_processes_recursive(t_process_data *data, int i)
{
    if (i >= data->cmd_count)
        return (0);
    data->pids[i] = fork();
    if (data->pids[i] == -1)
    {
        // perror("fork"); // DEBUG: Print error if fork fails
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
        return (-1);
    }
    if (data->pids[i] == 0)
    {
        reset_child_signal_handlers();
        redirect_io(data->cmds[i]->input_file, data->cmds[i]->output_file, 
				data->cmds[i]->output_mode);
        if (i > 0)
            dup2(data->pipes[i - 1][0], STDIN_FILENO);
        if (i < data->cmd_count - 1)
            dup2(data->pipes[i][1], STDOUT_FILENO);
        close_free_pipes_recursively(data->pipes, 0, data->cmd_count - 1);
		// ** THE FIX IS HERE **
        // If there are no arguments after redirection, it's an empty command. Exit successfully.
        if (data->cmds[i]->args[0] == NULL)
            exit(0);
        if (is_builtin(data->cmds[i]->args[0]))
        {
            exit(run_builtin(data->cmds[i]->args, data));
        }
        else
        {
            execute_cmd(data->cmds[i]->cmd_path, data->cmds[i]->args, data->path_dirs, data->env_list);
        }
        // perror("execve"); // DEBUG: Print error if execve fails
		ft_error_and_exit("execve", strerror(errno), EXIT_FAILURE);
        // exit(EXIT_FAILURE);
    }
    return fork_all_processes_recursive(data, i + 1);
}

/**
 * @brief Creates and initializes pipe file descriptors for a command pipeline.
 *
 * Allocates memory for and creates `cmd_count - 1` pipes to connect a
 * sequence of commands in a pipeline. Each pipe is represented as an
 * integer array of two file descriptors. Uses a recursive helper
 * (`create_all_pipes_recursively`) to create the pipes.
 * If memory allocation or pipe creation fails, the function cleans up and
 * returns NULL.
 * @param cmd_count The number of commands in the pipeline.
 * @return A pointer to an array of pipe file descriptor pairs,
 *         or NULL if no pipes are needed or if an error occurs.
 */
int **create_pipe_fds_between_commands(int cmd_count)
{
    int **pipes;
	int i;

    if (cmd_count < 2)
        return (NULL);
    pipes = malloc(sizeof(int *) * (cmd_count - 1));
    if (pipes == NULL)
        return (NULL);
    if (create_all_pipes_recursively(pipes, 0, cmd_count - 1) == -1)
    {
        i = 0;
        while (i < cmd_count - 1)
        {
            if (pipes[i])
                free(pipes[i]);
            i++;
        }
        free(pipes);
        return (NULL);
    }
    return (pipes);
}

/**
 * @brief Prepares and forks a pipeline of commands for execution.
 * 
 * Initializes the pipe file descriptors and child process ID storage based on the
 * number of commands. It sets up the `t_process_data` structure, allocating memory
 * and assigning relevant values. If any step fails (e.g., pipe creation or memory
 * allocation), it returns -1. Otherwise, it launches all child processes using
 * recursive forking.
 * @param data        Pointer to a t_process_data struct that will be filled with
 *                    pipeline-related data (commands, pipes, pids, etc.).
 * @param cmds        Array of pointers to parsed command structures.
 * @param cmd_count   Number of commands in the pipeline.
 * @param path_dirs   Array of directories to resolve command paths.
 * @return Returns 1 on successful setup and forking of all processes,
 *         0 if no commands need to be executed,
 *         or -1 on error (pipe creation, memory allocation, or forking failure).
 */
static int setup_and_fork_pipeline(t_process_data *data, t_command **cmds, int cmd_count, 
	char **path_dirs, t_env_var *env_list)
{
    if (cmd_count < 1)
        return (0);
    data->pipes = NULL;
    data->pids = NULL;
    if (cmd_count > 1)
    {
        data->pipes = prepare_pipe_fds(cmd_count);
        if (data->pipes == NULL)
            return (-1);
    }
    data->pids = create_child_processes(cmd_count, data->pipes);
    if (data->pids == NULL)
	{
        return (-1);
	}
    data->cmds = cmds;
    data->path_dirs = path_dirs;
    data->cmd_count = cmd_count;
	data->env_list = env_list; // Store the environment variables for built-in commands
    if (fork_all_processes_recursive(data, 0) == -1)
        return (-1);
    return (1); // Success
}

/**
 * @brief Executes a pipeline of commands with proper pipe and process management.
 * * This function sets up necessary pipes between commands, forks child processes
 * for each command in the pipeline, and ensures proper execution and
 * synchronization. It handles:
 * - Pipe file descriptor allocation and cleanup.
 * - Process forking for each command via recursion.
 * - Waiting for all child processes to finish.
 * In case of setup or forking errors, it cleans up resources and returns -1.
 * @param cmds        Array of pointers to parsed command structures.
 * @param cmd_count   Total number of commands in the pipeline.
 * @param path_dirs   Array of directories used to resolve command paths.
 * @return The exit status of the last command in the pipeline on success,
 * or -1 on failure (e.g., memory allocation or forking error).
 */
int run_command_pipeline(t_command **cmds, int cmd_count, char **path_dirs, t_env_var *env_list)
{
    t_process_data data;
    int fork_status;
	int exit_status;
	int signal_printed; // EXIT CODE DEBUG: flag to track if a signal message has been printed

	signal_printed = 0;
	// EXIT CODE DEBUG
	// Temporarily ignore signals in the parent shell
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    fork_status = setup_and_fork_pipeline(&data, cmds, cmd_count, path_dirs, env_list);
    if (fork_status == -1)
    {
        free(data.pids);
        close_free_pipes_recursively(data.pipes, 0, cmd_count - 1);
		setup_signal_handlers(); // Restore handlers on error
        return (-1);
    }
    close_free_pipes_recursively(data.pipes, 0, cmd_count - 1);
	exit_status = wait_all_children(data.pids, 0, cmd_count, 0, &signal_printed);
	// EXIT CODE DEBUG
	 // Restore the parent's original signal handlers after waiting is complete
    setup_signal_handlers();
	data.last_exit_status = exit_status;
    free(data.pids);
    return (exit_status);
}