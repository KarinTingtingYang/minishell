/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 09:35:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>
#include <errno.h>

/* -------------------------------------------------------------------------- */
/*                               Local utilities                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Safely obtain a printable command name.
 *
 * Returns a writable pointer so it matches ft_error_and_exit(char*,...).
 * If args[0] exists, a copy is placed in @p buf; otherwise a static
 * fallback "minishell" is returned.
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

static void	free_args_env(char **args, char **envp)
{
	if (args)
		free_split(args);
	if (envp)
		free_split(envp);
}

/* -------------------------------------------------------------------------- */
/*                          Pre-exec validity checks                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Handles errors from stat()/access and directory cases.
 *        Frees envp/args before exiting via the error helpers.
 *
 * @param cmd_path Full command path (must be non-NULL when called).
 * @param args     argv-style array for the command.
 * @param envp     envp array created for execve.
 * @return 1 if command is OK to exec, 0 otherwise (unreachable if error exits).
 */
int	perform_command_checks(char *cmd_path, char **args, char **envp)
{
	struct stat	st;
	int			e;

	if (stat(cmd_path, &st) == -1)
	{
		e = errno;
		handle_stat_error(envp, args, e); /* frees envp/args + exits */
		return (0);
	}
	if (S_ISDIR(st.st_mode))
	{
		char	namebuf[PATH_MAX];
		char	*name = safe_cmd_name(args, namebuf, sizeof(namebuf));

		free_args_env(args, envp);
		ft_error_and_exit(name, "Is a directory", 126);
		return (0);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		e = errno;
		handle_access_error(envp, args, e); /* frees envp/args + exits */
		return (0);
	}
	return (1);
}

/* -------------------------------------------------------------------------- */
/*                               Exec wrapper                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Executes a command via execve() after checks. On any failure,
 *        prints a proper diagnostic and exits. Ensures envp/args are freed
 *        inside the error handlers.
 */
void	execute_cmd(char *cmd_path, char **args, char **path_dirs,
	t_env_var *env_list)
{
	char	**envp;
	int		e;

	(void)path_dirs;
	reset_child_signal_handlers();
	envp = env_list_to_array(env_list);
	if (cmd_path == NULL)
	{
		/* Frees envp/args and exits. */
		handle_null_cmd_path(args, envp);
		return ;
	}
	if (!perform_command_checks(cmd_path, args, envp))
		return ;
	execve(cmd_path, args, envp);

	/* execve failed: capture errno, then free envp/args and exit */
	e = errno;
	handle_execve_error(envp, args, e);
}

/* -------------------------------------------------------------------------- */
/*                        Single command (no pipeline)                         */
/* -------------------------------------------------------------------------- */

int	handle_single_command(char *line, t_env_var *env_list,
								t_process_data *process_data)
{
	t_token	**tokens;
	char	**args;
	int		result;
	int		prev;

	tokens = parse_line(line, process_data);
	if (tokens == NULL)
	{
		if (process_data != NULL)
			return (process_data->last_exit_status);
		return (0);
	}
	args = expand_and_split_args(tokens, process_data);
	free_tokens(tokens);
	if (process_data && process_data->syntax_error)
		return (free_split(args), process_data->last_exit_status);
	if (args == NULL || args[0] == NULL)
	{
		prev = 0;
		if (process_data)
			prev = process_data->last_exit_status;
		return (free_split(args), prev);
	}
	result = execute_single_command(args, env_list, process_data);
	return (free_split(args), result);
}

/* -------------------------------------------------------------------------- */
/*                                 Pipeline                                    */
/* -------------------------------------------------------------------------- */

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
		/* Nothing to run (parse error, empty pipeline, or interrupted). */
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
	process_data->path_dirs = find_path_dirs(env_list);
	status = run_command_pipeline(process_data);
	cleanup_pipeline_resources(process_data);
	process_data->last_exit_status = status;
	return (status);
}

/* -------------------------------------------------------------------------- */
/*                             Entry point dispatcher                          */
/* -------------------------------------------------------------------------- */

int	execute_command(char *line, t_env_var *env_list,
	t_process_data *process_data)
{
	if (!check_heredoc_limit(line))
		return (2);
	if (is_unquoted_pipe_present(line))
		return (handle_pipeline_command(line, env_list, process_data));
	return (handle_single_command(line, env_list, process_data));
}
