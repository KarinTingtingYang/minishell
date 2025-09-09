/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:07:14 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/09 13:17:27 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Safely obtain a printable command name.
 * 
 * Returns a writable pointer so it matches ft_error_and_exit(char*,...).
 * If args[0] exists, a copy is placed in, otherwise a static
 * fallback "minishell" is returned.
 * @param args The command arguments array.
 * @param buf  Buffer to store the command name copy.
 * @param buflen Size of the buffer.
 * @return A pointer to the command name string.
 */
static const char	*safe_cmd_name(char **args, char *buf, size_t buflen)
{
	if (args && args[0] && buflen > 0)
	{
		ft_strlcpy(buf, args[0], buflen);
		return (buf);
	}
	return ("minishell");
}

/**
 * @brief Handles errors from stat()/access and directory cases.
 *        Frees envp/args before exiting via the error helpers.
 *
 * @param cmd_path Full command path (must be non-NULL when called).
 * @param args     argv-style array for the command.
 * @param envp     envp array created for execve.
 * @return 1 if command is OK to exec, 0 otherwise (unreachable
 *         if error exits).
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
	const char	*name;

	name = safe_cmd_name(args, namebuf, sizeof(namebuf));
	free_args_env(args, envp);
	if (error_code == EACCES)
		ft_error_and_exit((char *)name, "Permission denied", 126);
	else if (error_code == ENOTDIR)
		ft_error_and_exit((char *)name, "Not a directory", 126);
	ft_error_and_exit((char *)name, strerror(error_code), 126);
}

/**
 * @brief Handles errors from execve() system call.
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
 * @brief Handles redirection errors and sets appropriate exit status.
 *
 * This function checks for signals and syntax errors related to
 * redirections in the command arguments. It updates the last exit
 * status in the process data accordingly.
 *
 * @param data Pointer to the execution data (not used here).
 * @param process_data Pointer to the process data structure.
 * @param args The command arguments array.
 * @return The exit status code to be used by the caller.
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
