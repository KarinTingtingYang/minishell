/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 11:55:08 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 10:09:07 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks for and handles errors from the `dup` system call.
 *
 * This function verifies if `dup` failed to duplicate standard input or
 * output file descriptors. If an error is detected (indicated by a return
 * value of -1), it cleans up any file descriptors that were successfully
 * duplicated before the error occurred. It then reports the error using
 * `ft_error_and_exit` and terminates the program with a failure status.
 *
 * @param saved_stdin A pointer to the file descriptor for standard input.
 * @param saved_stdout A pointer to the file descriptor for standard output.
 */
static void	check_and_handle_dup_error(int *saved_stdin, int *saved_stdout)
{
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		ft_error_and_exit("dup", strerror(errno), EXIT_FAILURE);
	}
}

/**
 * @brief Sets up I/O redirection for a built-in command.
 *
 * Saves standard I/O file descriptors if redirection is needed.
 * Applies the new redirections and returns 0 on success, or 1 on failure.
 *
 * @param data The execution data with I/O files.
 * @param saved_stdin A pointer to store the saved standard input
 *                    file descriptor.
 * @param saved_stdout A pointer to store the saved standard output
 *                     file descriptor.
 * @param did_save A pointer to a flag indicating if the descriptors were saved.
 * @return 0 on success, 1 on error.
 */
int	setup_builtin_io(t_execute_data *data, int *saved_stdin,
							int *saved_stdout, int *did_save)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	*did_save = 0;
	if (data->input_file != NULL || data->output_file != NULL)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		check_and_handle_dup_error(saved_stdin, saved_stdout);
		*did_save = 1;
		if (apply_builtin_redirection(data->input_file,
				data->output_file, data->output_mode) == -1)
			return (1);
	}
	return (0);
}

/**
 * @brief Restores standard input and output file descriptors
 *        after redirection.
 *
 * This function reverses the effects of I/O redirection
 * for a builtin command by restoring the original standard input
 * and output streams. It first checks if any file descriptors were
 * saved before attempting to restore. It uses `dup2` to duplicate
 * the saved file descriptors back to their original locations
 * (`STDIN_FILENO` and `STDOUT_FILENO`), handling potential
 * errors during the process. Finally, it closes the saved file
 * descriptors to prevent resource leaks.
 *
 * @param did_save A flag indicating whether the original
 *                 file descriptors were saved.
 * @param saved_stdin The saved file descriptor for standard input.
 * @param saved_stdout The saved file descriptor for standard output.
 */
void	restore_builtin_io(int did_save, int saved_stdin, int saved_stdout)
{
	if (!did_save)
		return ;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Executes a built-in shell command with proper I/O redirection.
 *
 * This function handles the execution of a built-in command, such
 * as `cd` or `echo`, by first setting up any necessary input/output
 * redirection (e.g., redirecting standard output to a file). After
 * the I/O is set, it executes the built-in command and then restores
 * the original standard input and output streams.
 *
 * @param data A pointer to the execution data structure, which includes
 * the command's clean arguments and I/O files.
 * @param process_data A pointer to the overall process data.
 * @return The exit status of the executed built-in command.
 */
int	execute_builtin_command(t_execute_data *data, t_process_data *process_data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	did_save;
	int	status;

	status = setup_builtin_io(data, &saved_stdin, &saved_stdout, &did_save);
	if (status == 0)
		status = run_builtin(data->clean_args, process_data);
	restore_builtin_io(did_save, saved_stdin, saved_stdout);
	return (status);
}
