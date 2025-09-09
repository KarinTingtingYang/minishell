/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:07:14 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/09 09:36:11 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h> /* PATH_MAX */

/* -------------------------------------------------------------------------- */
/*                          Internal cleanup utilities                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Safely pick a command name for error printing.
 *
 * Copies args[0] into a stack buffer so we can free args/envp before
 * calling ft_error_and_exit(), avoiding using freed pointers.
 *
 * @param args   Argument vector (may be NULL).
 * @param buf    Stack buffer to write into.
 * @param buflen Size of @p buf.
 * @return const char*  Pointer to either @p buf (if args[0] existed) or "minishell".
 */
static const char	*safe_cmd_name(char **args, char *buf, size_t buflen)
{
	if (args && args[0] && buflen > 0)
	{
		/* Use your project's ft_strlcpy if available; fallback to snprintf otherwise */
		ft_strlcpy(buf, args[0], buflen);
		return (buf);
	}
	return ("minishell");
}

/**
 * @brief Free both args and envp if present.
 */
static void	free_args_env(char **args, char **envp)
{
	if (args)
		free_split(args);
	if (envp)
		free_split(envp);
}

/* -------------------------------------------------------------------------- */
/*                              Public interfaces                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Handles errors related to stat() system call.
 *
 * Frees args/envp before exiting. Uses a stack copy of the command name.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed stat() call.
 */
void	handle_stat_error(char **envp, char **args, int error_code)
{
	char		namebuf[PATH_MAX];
	const char	*name = safe_cmd_name(args, namebuf, sizeof(namebuf));

	free_args_env(args, envp);
	if (error_code == ENOTDIR)
		ft_error_and_exit((char *)name, "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit((char *)name, "No such file or directory", 127);
	ft_error_and_exit((char *)name, strerror(error_code), 126);
}

/**
 * @brief Handles errors related to access() system call.
 *
 * Frees args/envp before exiting. Uses a stack copy of the command name.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed access() call.
 */
void	handle_access_error(char **envp, char **args, int error_code)
{
	char		namebuf[PATH_MAX];
	const char	*name = safe_cmd_name(args, namebuf, sizeof(namebuf));

	free_args_env(args, envp);
	if (error_code == EACCES)
		ft_error_and_exit((char *)name, "Permission denied", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit((char *)name, "Not a directory", 126);
	ft_error_and_exit((char *)name, strerror(error_code), 126);
}

/**
 * @brief Handles errors related to execve() system call.
 *
 * Frees args/envp before exiting. Uses a stack copy of the command name.
 *
 * @param envp The environment variables array to free if needed.
 * @param args The command arguments array to free if needed.
 * @param error_code The error code from the failed execve() call.
 */
void	handle_execve_error(char **envp, char **args, int error_code)
{
	char		namebuf[PATH_MAX];
	const char	*name = safe_cmd_name(args, namebuf, sizeof(namebuf));

	free_args_env(args, envp);
	if (error_code == ENOEXEC)
		ft_error_and_exit((char *)name, "Exec format error", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit((char *)name, "Not a directory", 126);
	else if (error_code == ENOENT)
		ft_error_and_exit((char *)name, "No such file or directory", 127);
	else if (error_code == EACCES)
		ft_error_and_exit((char *)name, "Permission denied", 126);
	ft_error_and_exit((char *)name, strerror(error_code), 126);
}

/**
 * @brief Handles the case when the command path is NULL.
 *
 * If the user typed a path (contains '/'), report ENOENT. Otherwise,
 * report "command not found". Frees args/envp before exiting.
 *
 * @param args The command arguments array.
 * @param envp The environment variables array to free if needed.
 */
void	handle_null_cmd_path(char **args, char **envp)
{
	char		namebuf[PATH_MAX];
	int			has_slash;

	has_slash = (args && args[0] && ft_strchr(args[0], '/') != NULL);
	(void)has_slash; /* keep computed before freeing */

	/* Choose printable name before freeing inputs */
	if (args && args[0])
		ft_strlcpy(namebuf, args[0], sizeof(namebuf));

	/* Free everything we own before exiting */
	free_args_env(args, envp);

	if (has_slash)
		ft_error_and_exit(namebuf, "No such file or directory", 127);
	/* If no args[0], fall back to "minishell" as the subject */
	ft_error_and_exit((namebuf[0] ? namebuf : "minishell"),
		"command not found", 127);
}

/**
 * @brief Handles errors related to command redirection syntax.
 *
 * Sets the last exit status on syntax/signal errors. No allocations here.
 *
 * @param data Pointer to the execution data.
 * @param process_data Pointer to the process data.
 * @param args The command arguments array.
 * @return The exit status code based on the error encountered.
 */
int	handle_redirection_error(t_execute_data *data, t_process_data *process_data,
	char **args)
{
	int	last_arg_index;

	(void)data;
	if (g_signal_received == SIGINT)
	{
		process_data->last_exit_status = 130;
		return (130);
	}
	last_arg_index = 0;
	while (args && args[last_arg_index + 1] != NULL)
		last_arg_index++;
	if (args && is_redirection(args[last_arg_index]))
	{
		process_data->last_exit_status = 2;
		return (2);
	}
	process_data->last_exit_status = 1;
	return (1);
}
