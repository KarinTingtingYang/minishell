/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prepared_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:54:16 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/09 09:35:38 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../../libft/get_next_line/get_next_line.h"
#include <signal.h>
#include <errno.h>

/* -------------------------------------------------------------------------- */
/*                             Builtin in parent                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Handles the execution of a builtin command in the current process.
 *        Ensures all allocated memory in @p data is freed before returning.
 *
 * @param data Pointer to the execution data (will be freed here).
 * @param process_data Pointer to the process data (last_exit_status set).
 * @return The exit code of the builtin command.
 */
static int	execute_builtin_in_parent(t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	exit_code = run_builtin(data->clean_args, process_data);
	process_data->last_exit_status = exit_code;
	free_execute_data(data);
	return (exit_code);
}

/* -------------------------------------------------------------------------- */
/*                                Child logic                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Executes the child process logic after fork.
 *
 * Frees @p data before exiting in all paths that return control here.
 * For external commands, execute_cmd() should not return on success
 * (execve replaces the image). If it ever returns, we still free and exit.
 *
 * @param data Pointer to the execution data (freed here before exit/_exit).
 * @param process_data Pointer to the process data.
 */
static void	execute_child_process(t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	/* Child should not inherit the parent's SIGINT ignore: reset to default */
	signal(SIGINT, SIG_DFL);

	/* Clear any static buffers from GNL in the child */
	get_next_line_cleanup();

	/* Set up redirections; helper should exit on failure */
	redirect_io(data->input_file, data->output_file, data->output_mode);

	if (is_builtin(data->clean_args[0]))
	{
		exit_code = run_builtin(data->clean_args, process_data);
		free_execute_data(data);
		/* In a child after fork, prefer _exit to avoid atexit handlers twice */
		_exit(exit_code);
	}
	/* External command path: execve on success never returns */
	execute_cmd(data->cmd_path, data->clean_args, data->path_dirs, data->env_list);

	/* If we ever get here, exec failed but didn't exit inside helpers.
	   Fall back to last_exit_status (if set) or 127. */
	exit_code = (process_data->last_exit_status != 0)
		? process_data->last_exit_status : 127;
	free_execute_data(data);
	_exit(exit_code);
}

/* -------------------------------------------------------------------------- */
/*                                Parent logic                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Handles the parent process logic after forking.
 *
 * Frees @p data in the parent (the child's copy was freed or exec'd).
 *
 * @param pid The PID of the child process.
 * @param data Pointer to the execution data (freed here).
 * @param process_data Pointer to the process data.
 * @return The exit code of the command as observed by the parent.
 */
static int	handle_parent_process(pid_t pid, t_execute_data *data,
	t_process_data *process_data)
{
	int	exit_code;

	exit_code = wait_for_child_and_handle_status(pid);
	/* Parent no longer needs the prepared data */
	free_execute_data(data);
	/* Restore interactive shell signal handlers */
	setup_signal_handlers();
	process_data->last_exit_status = exit_code;
	return (exit_code);
}

/* -------------------------------------------------------------------------- */
/*                              Fork and execute                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Forks the process and executes the command.
 *
 * Ensures memory is released in both failure and success paths.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command, or -1 if fork failed (unreachable if
 *         ft_error_and_exit terminates the process).
 */
static int	fork_and_execute(t_execute_data *data, t_process_data *process_data)
{
	pid_t	pid;

	/* While forking, ignore SIGINT in parent to avoid double handling */
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		/* Free owned resources before exiting with error */
		free_execute_data(data);
		ft_error_and_exit("fork", strerror(errno), EXIT_FAILURE);
		return (-1);
	}
	else if (pid == 0)
	{
		/* Child branch */
		execute_child_process(data, process_data);
	}
	/* Parent branch */
	return (handle_parent_process(pid, data, process_data));
}

/* -------------------------------------------------------------------------- */
/*                         Public entry: execute prepared                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Executes a previously prepared command.
 *
 * Runs builtins in the parent when possible (no redirection and not
 * inside a pipeline) to match shell semantics and avoid forking overhead.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @return The exit code of the command.
 */
int	execute_prepared_command(t_execute_data *data, t_process_data *process_data)
{
	int	has_redirection;

	has_redirection = (data->input_file || data->output_file);
	if (is_builtin(data->clean_args[0]) && !has_redirection
		&& !process_data->in_pipeline)
		return (execute_builtin_in_parent(data, process_data));
	return (fork_and_execute(data, process_data));
}
