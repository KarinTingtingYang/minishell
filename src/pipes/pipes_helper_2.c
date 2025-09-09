/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes_helper_2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 12:01:47 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/09 13:16:23 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

/* -------------------------------------------------------------------------- */
/*                         Execute one command in child                       */
/* -------------------------------------------------------------------------- */
/**
 * @brief Executes a single command in a child process.
 *
 * Memory-safety notes:
 *  - For builtins: free all child-owned pipeline resources before _exit().
 *  - For external commands: we copy args[0] to a stack buffer (namebuf)
 *    so we can free pipeline resources + envp before printing errors.
 *  - On any failure path prior to execve() success, we free envp and
 *    call cleanup_child_pipeline_resources() to avoid still-reachable.
 */
static void	validate_command_path(char *cmd_path, char *cmd_name,
							char **envp, t_process_data *data)
{
	struct stat	st;
	int			e;

	if (stat(cmd_path, &st) == -1)
	{
		e = errno;
		handle_stat_error_child(cmd_name, envp, data, e);
	}
	if (S_ISDIR(st.st_mode))
	{
		free_split(envp);
		cleanup_child_pipeline_resources(data);
		ft_error_and_exit(cmd_name, "Is a directory", 126);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		e = errno;
		handle_access_error_child(cmd_name, envp, data, e);
	}
}

void	execute_child_command(t_command *cmd, t_process_data *data)
{
	char	namebuf[PATH_MAX];
	char	**envp;
	int		e;

	if (cmd->args == NULL || cmd->args[0] == NULL)
	{
		get_next_line_cleanup();
		cleanup_child_pipeline_resources(data);
		exit(0);
	}
	if (is_builtin(cmd->args[0]))
		handle_builtin_command(cmd, data);
	get_next_line_cleanup();
	ft_strlcpy(namebuf, cmd->args[0], sizeof(namebuf));
	envp = env_list_to_array(data->env_list);
	if (cmd->cmd_path == NULL)
		handle_path_resolution_error(namebuf, envp, data);
	validate_command_path(cmd->cmd_path, namebuf, envp, data);
	execve(cmd->cmd_path, cmd->args, envp);
	e = errno;
	handle_execve_error_child(namebuf, envp, data, e);
}

/* -------------------------------------------------------------------------- */
/*                         Wait helpers and final status                      */
/* -------------------------------------------------------------------------- */
/**
 * @brief Processes the exit status of a single child process.
 *
 * Updates last_status only for the last child in the pipeline.
 */
static void	process_child_status(int status, t_wait_info *info,
										int is_last_child)
{
	int	sig;

	if (WIFEXITED(status))
	{
		if (is_last_child)
			info->last_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (!(info->signal_printed))
		{
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			info->signal_printed = 1;
		}
		if (is_last_child)
			info->last_status = 128 + sig;
	}
}

/**
 * @brief Recursively waits for all children. On waitpid error, frees pids
 *        and exits with an error to avoid still-reachable memory.
 *
 * @param data  Process data (to free pids on fatal error).
 * @param index Current index in the pids array.
 * @param max   Total number of children.
 * @param info  Aggregate wait state.
 * @return Final status of the last child on success (does not return on error).
 */
static int	wait_all_children(t_process_data *data, int index, int max,
									t_wait_info *info)
{
	int	status;
	int	is_last_child;
	int	saved;

	if (index >= max)
		return (info->last_status);
	if (waitpid(data->pids[index], &status, 0) == -1)
	{
		saved = errno;
		free(data->pids);
		data->pids = NULL;
		ft_error_and_exit("waitpid", strerror(saved), EXIT_FAILURE);
	}
	is_last_child = (index == max - 1);
	process_child_status(status, info, is_last_child);
	return (wait_all_children(data, index + 1, max, info));
}

/* -------------------------------------------------------------------------- */
/*                         Pipeline run / parent side                         */
/* -------------------------------------------------------------------------- */
/**
 * @brief Executes the core logic of a command pipeline (parent side).
 *
 * Closes all pipes, waits for children, updates last status, and frees pids.
 */
int	run_pipeline_core(t_process_data *data, int cmd_count)
{
	int			exit_status;
	t_wait_info	wait_state;

	wait_state.last_status = 0;
	wait_state.signal_printed = 0;
	close_free_pipes_recursively(data->pipes, 0, cmd_count - 1);
	exit_status = wait_all_children(data, 0, cmd_count, &wait_state);
	data->last_exit_status = exit_status;
	free(data->pids);
	data->pids = NULL;
	return (exit_status);
}
