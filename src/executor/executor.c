/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:40:52 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>
#include <errno.h>

/**
 * @brief Safely retrieves the command name from args for error messages.
 *
 * This function copies the first argument (command name) into a provided
 * buffer if available, ensuring it is null-terminated. If args is NULL or
 * empty, it returns a fallback string "minishell".
 *
 * @param args The argument list (argv-style).
 * @param buf The buffer to copy the command name into.
 * @param buflen The size of the buffer.
 * @return A pointer to the command name in buf, or "minishell" if unavailable.
 */
static char	*safe_cmd_name(char **args, char *buf, size_t buflen)
{
	static char	fallback[] = "minishell";

	if (args && args[0] && buflen > 0)
	{
		ft_strlcpy(buf, args[0], buflen);
		return (buf);
	}
	return (fallback);
}

/**
 * @brief Handles errors from stat()/access and directory cases.
 *        Frees envp/args before exiting via the error helpers.
 *
 * @param cmd_path Full command path (must be non-NULL when called).
 * @param args     argv-style array for the command.
 * @param envp     envp array created for execve.
 */
int	perform_command_checks(char *cmd_path, char **args, char **envp)
{
	struct stat	st;
	int			e;
	char		namebuf[PATH_MAX];
	char		*name;

	if (stat(cmd_path, &st) == -1)
	{
		e = errno;
		handle_stat_error(envp, args, e);
		return (0);
	}
	if (S_ISDIR(st.st_mode))
	{
		name = safe_cmd_name(args, namebuf, sizeof(namebuf));
		free_args_env(args, envp);
		ft_error_and_exit(name, "Is a directory", 126);
		return (0);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		e = errno;
		handle_access_error(envp, args, e);
		return (0);
	}
	return (1);
}

/**
 * @brief Handles cleanup and exit status for a null command pipeline.
 *
 * This function is called when `prepare_pipeline_commands` returns NULL,
 * indicating a syntax error, an empty pipeline, or an interruption. It
 * frees the token parts and returns the correct exit status.
 *
 * @param process_data The process data structure.
 * @return The appropriate exit status (130 for SIGINT, 2 for errors).
 */
static int	handle_null_pipeline_error(t_process_data *process_data)
{
	if (process_data->parts)
	{
		free_split(process_data->parts);
		process_data->parts = NULL;
	}
	if (g_signal_received == SIGINT)
	{
		process_data->last_exit_status = 130;
		return (130);
	}
	process_data->last_exit_status = 2;
	return (2);
}

/**
 * @brief Orchestrates the parsing, validation, and execution of
 *        a command pipeline.
 *
 * @param line The input line containing the command(s) and pipes.
 * @param env_list The list of environment variables.
 * @param process_data The structure containing all process-related data.
 * @return The final exit status of the pipeline.
 */
int	handle_pipeline_command(char *line, t_env_var *env_list,
									t_process_data *process_data)
{
	int	status;

	process_data->parts = NULL;
	process_data->cmds = prepare_pipeline_commands(
			line, &process_data->cmd_count, &process_data->parts, process_data);
	if (process_data->syntax_error)
	{
		free_split(process_data->parts);
		process_data->parts = NULL;
		return (process_data->last_exit_status);
	}
	if (process_data->cmds == NULL)
	{
		return (handle_null_pipeline_error(process_data));
	}
	process_data->path_dirs = find_path_dirs(env_list);
	status = run_command_pipeline(process_data);
	cleanup_pipeline_resources(process_data);
	process_data->last_exit_status = status;
	return (status);
}

/**
 * @brief Executes a command line, handling both single commands and pipelines.
 *
 * This function checks for heredoc limits, determines if the command line
 * contains unquoted pipes, and delegates to the appropriate handler for
 * single commands or pipelines. It returns the exit status of the executed
 * command(s).
 *
 * @param line The command line to execute.
 * @param env_list The linked list of environment variables.
 * @param process_data Pointer to the process data structure.
 * @return The exit status of the executed command(s),
 *         or 2 on heredoc limit error.
 */
int	execute_command(char *line, t_env_var *env_list,
	t_process_data *process_data)
{
	if (!check_heredoc_limit(line))
		return (2);
	if (is_unquoted_pipe_present(line))
		return (handle_pipeline_command(line, env_list, process_data));
	return (handle_single_command(line, env_list, process_data));
}
